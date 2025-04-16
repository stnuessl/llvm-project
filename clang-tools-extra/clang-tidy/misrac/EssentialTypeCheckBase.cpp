//===--- EssentialTypeCheckBase.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "EssentialTypeCheckBase.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

namespace {

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(EssentialTypeCheckBase *Visitor) : Visitor(Visitor) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    const auto *Info = MD.getMacroInfo();

    if (Info->isFunctionLike()) {
      if (!Args)
        return;

      for (unsigned i = 0; i < Args->getNumMacroArguments(); ++i) {
        const auto *Token = Args->getUnexpArgument(i);

        const auto *ArgumentIdentifierInfo = Token->getIdentifierInfo();
        if (!ArgumentIdentifierInfo)
          continue;

        if (!ArgumentIdentifierInfo->hasMacroDefinition())
          continue;

        if (!isBoolIdentifier(ArgumentIdentifierInfo))
          return;

        Visitor->visitBoolExpansion(Token->getLocation());
      }

      return;
    }

    if (!isBoolIdentifier(MacroNameTok.getIdentifierInfo()))
      return;

    Visitor->visitBoolExpansion(Range.getBegin());
  }

private:
  static bool isBoolIdentifier(const IdentifierInfo *II) {
    auto Name = II->getName();

    return Name == "false" || Name == "true";
  }

  EssentialTypeCheckBase *Visitor;
};

std::optional<StringRef> getSharedOption(const ClangTidyContext *Context,
                                         StringRef Name) {
  SmallString<64> Buffer;
  auto FullName = Twine("misrac.shared-options.", Name).toStringRef(Buffer);

  auto &Map = Context->getOptions().CheckOptions;
  auto It = Map.find(FullName);

  if (It != Map.end())
    return It->getValue().Value;

  return std::nullopt;
}

StringRef getSharedOption(const ClangTidyContext *Context, StringRef Name,
                          StringRef Default) {
  return getSharedOption(Context, Name).value_or(Default);
}

} // namespace

EssentialTypeCheckBase::EssentialTypeCheckBase(StringRef Name,
                                               ClangTidyContext *Context)
    : MisracBaseCheck(Name, Context), BooleanLocations(64), Booleans(8) {

  auto ValueList = getSharedOption(Context, "TreatAsBoolean", "").trim();

  while (!ValueList.empty()) {
    auto [Head, Tail] = ValueList.split(';');

    Head = Head.trim();

    if (!Head.empty())
      Booleans.insert(Head);

    ValueList = Tail;
  }
}

void EssentialTypeCheckBase::addPPCallbacks(const SourceManager &SM,
                                            Preprocessor *PP,
                                            Preprocessor *ModuleExpanderPP) {

  (void)SM;
  (void)ModuleExpanderPP;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));
}

void EssentialTypeCheckBase::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off

  Finder->addMatcher(
    expr(
      unless(hasParent(expr())),
      unless(hasParent(varDecl())),
      unless(hasParent(initListExpr())),
      unless(hasParent(returnStmt()))
    ).bind("expr"),
    this);

  Finder->addMatcher(
    varDecl(
      hasInitializer(expr())
    ).bind("var"),
    this);

  Finder->addMatcher(
    functionDecl(
      unless(returns(voidType()))
    ).bind("fn"),
    this);

  Finder->addMatcher(
    returnStmt(
      hasReturnValue(expr())
    ).bind("ret"),
    this);
  // clang-format on
}

void EssentialTypeCheckBase::visitBoolExpansion(SourceLocation Loc) {
  Loc = getSourceManager().getFileLoc(Loc);

  (void)BooleanLocations.insert(Loc);
}

void EssentialTypeCheckBase::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  if (const auto *E = Result.Nodes.getNodeAs<Expr>("expr")) {
    dispatch(E);
    return;
  }

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    dispatch(VD);
    return;
  }

  if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("fn")) {
    dispatch(FD);
    return;
  }

  if (const auto *RS = Result.Nodes.getNodeAs<ReturnStmt>("ret")) {
    dispatch(RS);
    return;
  }
}

EssentialType EssentialTypeCheckBase::visit(const ArraySubscriptExpr *Expr) {
  const auto *Idx = Expr->getIdx();
  auto ET = visit(Idx);

  checkArraySubscript(Idx, ET);

  return getEssentialType(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const BinaryOperator *Expr) {
  auto ET1 = visit(Expr->getLHS());
  auto ET2 = visit(Expr->getRHS());

  checkBinaryOperands(Expr, ET1, ET2);

  switch (Expr->getOpcode()) {
  case BO_Comma:
    return ET2;
  case BO_LE:
  case BO_GE:
  case BO_GT:
  case BO_LT:
  case BO_EQ:
  case BO_NE:
  case BO_Cmp:
    visitOperation(Expr, ET1, ET2);
    return EssentialType::createBoolean();
  case BO_LOr:
  case BO_LAnd:
    return EssentialType::createBoolean();
  case BO_Assign:
    visitAssignment(Expr, ET1, ET2);
    return ET1;
  case BO_MulAssign:
  case BO_DivAssign:
  case BO_RemAssign:
  case BO_AndAssign:
  case BO_OrAssign:
  case BO_XorAssign:
    visitAssignment(Expr, ET1, ET2);
    visitOperation(Expr, ET1, ET2);
    return ET1;
  case BO_Add:
    if (ET2.isChar() && ET1.isInteger())
      return ET2;

    LLVM_FALLTHROUGH;
  case BO_Sub:
    if (ET1.isChar() && ET2.isInteger())
      return ET1;

    if (ET1.isChar() || ET2.isChar())
      checkAdditiveOpChar(Expr, ET1, ET2);

    visitOperation(Expr, ET1, ET2);
    return getEssentialTypeUACOperator(Expr, ET1, ET2);
  case BO_AddAssign:
    if (ET2.isChar() && ET1.isInteger())
      return ET2;

    LLVM_FALLTHROUGH;
  case BO_SubAssign:
    if (ET1.isChar() && ET2.isInteger())
      return ET1;

    if (ET1.isChar() || ET2.isChar())
      checkAdditiveOpChar(Expr, ET1, ET2);

    visitAssignment(Expr, ET1, ET2);
    visitOperation(Expr, ET1, ET2);
    return ET1;
  case BO_Mul:
  case BO_Div:
  case BO_Rem:
  case BO_And:
  case BO_Or:
  case BO_Xor:
    visitOperation(Expr, ET1, ET2);
    return getEssentialTypeUACOperator(Expr, ET1, ET2);
  case BO_ShlAssign:
  case BO_ShrAssign:
    visitAssignment(Expr, ET1, ET2);

    LLVM_FALLTHROUGH;
  case BO_Shl:
  case BO_Shr:
    if (ET1.isUnsigned()) {
      if (auto Option = getEssentialTypeIntegerConstantExpr(Expr))
        return Option.value();

      return getEssentialType(Expr->getLHS());
    }
    break;
  case BO_PtrMemD:
  case BO_PtrMemI:
    break;
  }

  return getEssentialType(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const CallExpr *Expr) {
  // Retrieve the function type from the callee expression.
  const auto *CalleeExpr = Expr->getCallee()->IgnoreCasts();
  auto CalleeType = CalleeExpr->getType();

  if (CalleeType->isFunctionPointerType())
    CalleeType = CalleeType->getPointeeType();

  const auto *FnProtoType = CalleeType->getAs<FunctionProtoType>();
  if (!FnProtoType)
    return getEssentialType(Expr);

  // Iterate over the parameter types and argument expression types and
  // check their essential types.
  auto ParamTypes = FnProtoType->param_types();

  const auto *TypeIt = ParamTypes.begin();
  const auto *TypeEnd = ParamTypes.end();
  auto ExprIt = Expr->arg_begin();
  auto ExprEnd = Expr->arg_end();

  while (TypeIt != TypeEnd && ExprIt != ExprEnd) {

    auto ET1 = getEssentialType(*TypeIt);
    auto ET2 = visit(*ExprIt);

    if (ET1.isValid())
      visitAssignment(*ExprIt, ET1, ET2);

    ++TypeIt;
    ++ExprIt;
  }

  return getEssentialType(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const CharacterLiteral *Expr) {
  return EssentialType::create(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const ConditionalOperator *Expr) {
  const auto *Cond = Expr->getCond();
  auto ET1 = visit(Cond);
  auto ET2 = visit(Expr->getLHS());
  auto ET3 = visit(Expr->getRHS());

  checkConditionalOperand(Cond, ET1);

  if (ET2 != ET3) {
    checkOperation(Expr, ET2, ET3);

    if (const auto *LHS = Expr->getLHS(); isCompositeExpr(LHS))
      checkOperationWithComposite(Expr, LHS, ET1, ET2);

    if (const auto *RHS = Expr->getRHS(); isCompositeExpr(RHS))
      checkOperationWithComposite(Expr, RHS, ET2, ET1);
  }

  if (ET2 == ET3)
    return ET2;

  if (ET2.isSameIntegerCategory(ET3)) {
    if (ET3.isWider(ET2))
      return ET3;

    return ET2;
  }

  return getEssentialType(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const DeclRefExpr *Expr) {
  const auto *Decl = Expr->getDecl();

  if (const auto *ECD = dyn_cast<EnumConstantDecl>(Decl)) {
    const auto *Parent = ECD->getDeclContext();
    const auto *ED = dyn_cast<EnumDecl>(Parent);

    return EssentialType::create(ED);
  }

  return getEssentialType(Decl->getType());
}

EssentialType EssentialTypeCheckBase::visit(const ExplicitCastExpr *Expr) {
  const auto *SE = Expr->getSubExpr();

  auto ET1 = getEssentialType(Expr);
  auto ET2 = visit(SE);

  // A cast to 'void' is always allowed.
  if (Expr->getCastKind() == CK_ToVoid)
    return ET1;

  // The essential type system does not forbid casting to pointers.
  if (Expr->getType()->isPointerType())
    return ET1;

  visitCast(Expr, ET1, ET2);

  return ET1;
}

EssentialType EssentialTypeCheckBase::visit(const Expr *Expr) {
  if (const auto *BO = dyn_cast<BinaryOperator>(Expr))
    return visit(BO);

  if (const auto *DRE = dyn_cast<DeclRefExpr>(Expr))
    return visit(DRE);

  if (const auto *CE = dyn_cast<CallExpr>(Expr))
    return visit(CE);

  if (const auto *PE = dyn_cast<ParenExpr>(Expr))
    return visit(PE);

  if (const auto *ASE = dyn_cast<ArraySubscriptExpr>(Expr))
    return visit(ASE);

  if (const auto *ICE = dyn_cast<ImplicitCastExpr>(Expr))
    return visit(ICE);

  if (const auto *UO = dyn_cast<UnaryOperator>(Expr))
    return visit(UO);

  if (const auto *CO = dyn_cast<ConditionalOperator>(Expr))
    return visit(CO);

  if (const auto *ECE = dyn_cast<ExplicitCastExpr>(Expr))
    return visit(ECE);

  if (const auto *FE = dyn_cast<FullExpr>(Expr))
    return visit(FE);

  if (const auto *CL = dyn_cast<CharacterLiteral>(Expr))
    return visit(CL);

  if (const auto *IL = dyn_cast<IntegerLiteral>(Expr))
    return visit(IL);

  if (const auto *UETT = dyn_cast<UnaryExprOrTypeTraitExpr>(Expr))
    return visit(UETT);

  if (const auto *SL = dyn_cast<StringLiteral>(Expr))
    return visit(SL);

  if (const auto *RE = dyn_cast<RecoveryExpr>(Expr))
    return visit(RE);

  return getEssentialType(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const FullExpr *Expr) {
  return visit(Expr->getSubExpr());
}

EssentialType EssentialTypeCheckBase::visit(const ImplicitCastExpr *Expr) {
  return visit(Expr->getSubExpr());
}

EssentialType EssentialTypeCheckBase::visit(const IntegerLiteral *Expr) {
  if (isBooleanLocation(Expr->getExprLoc()))
    return EssentialType::createBoolean();

  return EssentialType::create(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const ParenExpr *Expr) {
  return visit(Expr->getSubExpr());
}

EssentialType EssentialTypeCheckBase::visit(const StringLiteral *Expr) {
  return EssentialType::create(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const RecoveryExpr *Expr) {
  return EssentialType::create(Expr);
}

EssentialType
EssentialTypeCheckBase::visit(const UnaryExprOrTypeTraitExpr *Expr) {
  if (!Expr->isArgumentType())
    (void)visit(Expr->getArgumentExpr());

  if (auto Option = getEssentialTypeIntegerConstantExpr(Expr))
    return Option.value();

  return getEssentialType(Expr);
}

EssentialType EssentialTypeCheckBase::visit(const UnaryOperator *Expr) {
  auto ET = visit(Expr->getSubExpr());

  checkUnaryOperand(Expr, ET);

  switch (Expr->getOpcode()) {
  case UO_LNot:
    return EssentialType::createBoolean();
  case UO_Not:
    if (ET.isUnsigned()) {
      if (auto Option = getEssentialTypeIntegerConstantExpr(Expr))
        return Option.value();

      return ET;
    }

    break;
  case UO_Plus:
  case UO_PreInc:
  case UO_PostInc:
    if (ET.isInteger())
      return ET;

    break;
  case UO_Minus:
    if (ET.isSigned()) {
      if (auto Option = getEssentialTypeIntegerConstantExpr(Expr))
        return Option.value();

      return ET;
    }

    break;
  case UO_PreDec:
  case UO_PostDec:
    if (ET.isSigned())
      return ET;

    break;
  case UO_AddrOf:
  case UO_Deref:
  case UO_Real:
  case UO_Imag:
  case UO_Extension:
  case UO_Coawait:
    break;
  }

  return getEssentialType(Expr);
}

void EssentialTypeCheckBase::dispatch(const Expr *Expr) { (void)visit(Expr); }

void EssentialTypeCheckBase::dispatch(const VarDecl *Decl) {
  visitInitExpr(Decl->getType(), Decl->getInit());
}

void EssentialTypeCheckBase::dispatch(const FunctionDecl *Decl) {
  ReturnType = Decl->getReturnType();
}

void EssentialTypeCheckBase::dispatch(const ReturnStmt *Stmt) {
  const auto *Expr = Stmt->getRetValue();

  auto ET1 = getEssentialType(ReturnType);
  auto ET2 = visit(Expr);

  visitAssignment(Expr, ET1, ET2);
}

void EssentialTypeCheckBase::visitAssignment(const Expr *Expr,
                                             EssentialType ET1,
                                             EssentialType ET2) {
  if (ET1 == ET2)
    return;

  checkAssignment(Expr, ET1, ET2);

  if (isCompositeExpr(Expr))
    checkAssignmentWithComposite(Expr, ET1, ET2);
}

void EssentialTypeCheckBase::visitAssignment(const BinaryOperator *Expr,
                                             EssentialType ET1,
                                             EssentialType ET2) {
  if (ET1 == ET2)
    return;

  checkAssignment(Expr, ET1, ET2);

  if (isCompositeExpr(Expr->getRHS()))
    checkAssignmentWithComposite(Expr, ET1, ET2);
}

void EssentialTypeCheckBase::visitOperation(const BinaryOperator *Expr,
                                            EssentialType ET1,
                                            EssentialType ET2) {

  if (ET1 == ET2)
    return;

  checkOperation(Expr, ET1, ET2);

  if (const auto *LHS = Expr->getLHS(); isCompositeExpr(LHS))
    checkOperationWithComposite(Expr, LHS, ET1, ET2);

  if (const auto *RHS = Expr->getRHS(); isCompositeExpr(RHS))
    checkOperationWithComposite(Expr, RHS, ET2, ET1);
}

void EssentialTypeCheckBase::visitCast(const ExplicitCastExpr *Expr,
                                       EssentialType ET1, EssentialType ET2) {
  if (ET1 == ET2)
    return;

  checkCast(Expr, ET1, ET2);

  if (isCompositeExpr(Expr->getSubExpr()))
    checkCastWithComposite(Expr, ET1, ET2);
}

void EssentialTypeCheckBase::visitInitExpr(QualType Type, const Expr *Expr) {
  if (Type->isScalarType()) {
    visitInitExprScalar(Type, Expr);
    return;
  }

  if (const auto *AT = dyn_cast<ArrayType>(Type)) {
    visitInitExprArray(AT, Expr);
    return;
  }

  if (const auto *RT = Type->getAs<RecordType>()) {
    visitInitExprRecord(RT, Expr);
    return;
  }
}

void EssentialTypeCheckBase::visitInitExprScalar(QualType Type,
                                                 const Expr *Expr) {
  auto ET1 = getEssentialType(Type);
  auto ET2 = visit(Expr);

  if (!ET1.isValid())
    return;

  visitAssignment(Expr, ET1, ET2);
}

void EssentialTypeCheckBase::visitInitExprArray(const ArrayType *Type,
                                                const Expr *Expr) {
  const auto *IL = dyn_cast<InitListExpr>(Expr);
  if (!IL)
    return;

  if (IL->isSyntacticForm() && IL->isIdiomaticZeroInitializer(getLangOpts()))
    return;

  auto ElementType = Type->getElementType();

  for (const auto *InitExpr : IL->inits())
    visitInitExpr(ElementType, InitExpr);
}

void EssentialTypeCheckBase::visitInitExprRecord(const RecordType *Type,
                                                 const Expr *Expr) {
  const auto *IL = dyn_cast<InitListExpr>(Expr);
  if (!IL)
    return;

  if (IL->isSyntacticForm() && IL->isIdiomaticZeroInitializer(getLangOpts()))
    return;

  auto Fields = Type->getDecl()->fields();
  auto FieldIt = Fields.begin();
  const auto FieldEnd = Fields.end();

  auto Inits = IL->inits();
  auto *ExprIt = Inits.begin();
  const auto *ExprEnd = Inits.end();

  while (FieldIt != FieldEnd && ExprIt != ExprEnd) {
    visitInitExpr((*FieldIt)->getType(), *ExprIt);

    ++FieldIt;
    ++ExprIt;
  }
}

std::optional<EssentialType>
EssentialTypeCheckBase::getEssentialTypeIntegerConstantExpr(
    const Expr *Expr) const {

  if (auto Option = Expr->getIntegerConstantExpr(getASTContext()))
    return EssentialType::create(Expr, Option.value());

  return std::nullopt;
}

EssentialType EssentialTypeCheckBase::getEssentialTypeUACOperator(
    const BinaryOperator *Expr, EssentialType ET1, EssentialType ET2) const {

  if (ET1.isSameIntegerCategory(ET2)) {
    if (auto Option = getEssentialTypeIntegerConstantExpr(Expr))
      return Option.value();

    if (ET2.isWider(ET1))
      return ET2;

    return ET1;
  }

  if (ET1.isChar() && ET2.isChar() && Expr->getOpcode() == BO_Sub)
    return ET1;

  return getEssentialType(Expr);
}

bool EssentialTypeCheckBase::isCompositeExpr(const Expr *Expr) const {
  Expr = Expr->IgnoreParenImpCasts();

  if (const auto *BO = dyn_cast<BinaryOperator>(Expr))
    return isCompositeExpr(BO);

  if (const auto *CO = dyn_cast<ConditionalOperator>(Expr))
    return isCompositeExpr(CO);

  return false;
}

bool EssentialTypeCheckBase::isCompositeExpr(const BinaryOperator *Expr) const {
  switch (Expr->getOpcode()) {
  case BO_Add:
  case BO_Sub:
  case BO_Mul:
  case BO_Div:
  case BO_Rem:
  case BO_And:
  case BO_Or:
  case BO_Xor:
  case BO_Shl:
  case BO_Shr:
    return !Expr->isEvaluatable(getASTContext(), Expr::SE_AllowSideEffects);
  default:
    return false;
  }
}

bool EssentialTypeCheckBase::isCompositeExpr(
    const ConditionalOperator *Expr) const {

  return isCompositeExpr(Expr->getLHS()) || isCompositeExpr(Expr->getRHS());
}

EssentialType EssentialTypeCheckBase::getEssentialType(QualType Type) const {
  auto Info = getASTContext().getTypeInfo(Type);

  return EssentialType::create(Type, Info);
}

EssentialType EssentialTypeCheckBase::getEssentialType(const Expr *Expr) const {
  return getEssentialType(Expr->getType());
}

bool EssentialTypeCheckBase::isBooleanLocation(SourceLocation Loc) const {
  if (!Loc.isMacroID())
    return false;

  Loc = getSourceManager().getFileLoc(Loc);

  return BooleanLocations.contains(Loc);
}

bool EssentialTypeCheckBase::isBooleanType(StringRef Name) const {
  if (Name.empty())
    return false;

  return Booleans.contains(Name);
}

bool EssentialTypeCheckBase::isBooleanType(QualType Type) const {
  if (const auto *TT = Type->getAs<TypedefType>())
    return isBooleanType(TT->getDecl()->getName());

  if (const auto *ET = Type->getAs<EnumType>())
    return isBooleanType(ET->getDecl()->getName());

  return false;
}

} // namespace clang::tidy::misrac
