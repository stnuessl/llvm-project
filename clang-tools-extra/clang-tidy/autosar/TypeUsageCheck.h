//===--- TypeUsageCheck.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_TYPEUSAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_TYPEUSAGECHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::autosar {

/// Checks the correct usage of AUTOSAR data types and their associated values.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/autosar/type-usage.html
class TypeUsageCheck : public ClangTidyCheck {
public:
  TypeUsageCheck(StringRef Name, ClangTidyContext *Context);

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.C99 || LangOpts.C11 || LangOpts.C17;
  }

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

  bool insertMacroDefinition(SourceLocation Loc, StringRef Value) {
    // Retrieve the type for the given value.
    auto FindIt = ValueTypeMap.find(Value);
    if (FindIt == ValueTypeMap.end())
      return true;

    Loc = SrcM->getFileLoc(Loc);

    auto [It, Ok] = LocTypeMap.insert({Loc, FindIt->second});
    return Ok || It != LocTypeMap.end();
  }

private:
  //
  // A 'TypeProxy' can either be a C type or an AUTOSAR type, as specified by
  // the checks configuration. As the type usage check is only interested in
  // AUTOSAR types, it will compare different C types as equal, thereby
  // effectively ignoring them (the compiler and/or the MISRA essential type
  // system checks are supposed to catch any issues related to the normal C
  // types).
  // If an instance of this class is marked as 'bool compatible', it is
  // effectively the result of a comparison operation or the logical not
  // operation. This information is used to conventiently allow the result of
  // such expressions (represented with the type 'int' in the abstract syntax
  // tree) to be directly assigned to any boolean value, as specified by this
  // checks configuration (default is 'boolean').
  // For this reason, a 'bool compatible' instance of this class acts as both,
  // a normal C type or an AUTOSAR type.
  //
  class TypeProxy {
  public:
    TypeProxy(StringRef Name, QualType Type) : Name(Name), Type(Type) {}
    TypeProxy(StringRef Name, const Expr *Expr)
        : TypeProxy(Name, Expr->getType()) {}
    explicit TypeProxy(QualType Type) : TypeProxy("", Type) {}
    explicit TypeProxy(const Expr *Expr) : TypeProxy(Expr->getType()) {}

    StringRef getName(llvm::SmallVectorImpl<char> &Buffer) const {
      if (!isCType())
        return Name;

      Buffer.clear();
      llvm::raw_svector_ostream OS(Buffer);
      OS << Type;

      return OS.str();
    }

    StringRef getName() const {
      if (!isCType())
        return Name;

      // The program is supposed to never get here.
      return "(unknown)";
    }

    bool operator==(const TypeProxy &Other) const { return Name == Other.Name; }

    bool operator!=(const TypeProxy &Other) const { return Name != Other.Name; }

    void setBoolCompatible() { Type = QualType(); }
    bool isBoolCompatible() const { return Type.isNull(); }

    bool isCType() const { return Name.empty() || isBoolCompatible(); }
    bool isAutosarType() const { return !Name.empty(); }

  private:
    StringRef Name;
    QualType Type;
  };

  bool addTypeValueMapping(StringRef Type, StringRef Value) {
    auto [It1, Ok1] = ValueTypeMap.insert({Value, Type});
    if (!Ok1)
      return It1 != ValueTypeMap.end();

    auto [It2, Ok2] = TypeSet.insert(Type);

    return Ok2 || It2 != TypeSet.end();
  }

  TypeProxy getTypeProxy(QualType Type) {
    // FIXME: Why getAs()? Why can I sometimes use dyn_cast()?
    if (const auto *TType = Type->getAs<TypedefType>()) {
      auto Name = TType->getDecl()->getName();

      if (TypeSet.contains(Name))
        return TypeProxy(Name, Type);

      return TypeProxy(Type);
    }

#if 0
    // FIXME: is it possible to also support enumerations and all associated
    // values?
    if (const auto *EType = Type->getAs<EnumType>()) {
      auto Name = EType->getDecl()->getName();

      if (TypeSet.contains(Name))
        return TypeProxy(Name, Type);

      return TypeProxy(Type);
    }
#endif

    return TypeProxy(Type);
  }

  TypeProxy getTypeProxy(const ValueDecl *Decl) {
    return getTypeProxy(Decl->getType());
  }

  StringRef getTypeName(SourceLocation Loc) {
    Loc = SrcM->getFileLoc(Loc);

    auto It = LocTypeMap.find(Loc);
    if (It == LocTypeMap.end())
      return "";

    return It->second;
  }

  template <typename UnaryFunction>
  void dispatchConfigOption(StringRef Key, UnaryFunction Dispatcher,
                            StringRef Default = "") {
    auto Value = Options.get(Key, Default);

    while (!Value.empty()) {
      auto [Head, Tail] = Value.split(';');

      Value = Tail;
      Head = Head.trim();

      if (Head.empty())
        continue;

      Dispatcher(Head);
    }
  }

  template <typename BinaryFunction>
  void dispatchEachConfigTypeValue(BinaryFunction Dispatcher) {
    auto TypeDispatcher = [this, Dispatcher](StringRef TypeName) {
      auto ValueDispatcher = [this, Dispatcher, TypeName](StringRef TypeValue) {
        Dispatcher(TypeName, TypeValue);
      };

      dispatchConfigOption(TypeName, ValueDispatcher);
    };

    // Use nested dispatchers to retrieve all values which are defined for each
    // type in check's configuration.
    dispatchConfigOption("Option.Add", TypeDispatcher);
  }

  bool checkCompatibility(const TypeProxy &LHS, const TypeProxy &RHS) {
    if (LHS == RHS)
      return true;

    if (BoolTypes.contains(LHS.getName()) && RHS.isBoolCompatible())
      return true;

    return false;
  }

  void dispatchFunctionDecl(const FunctionDecl *FunctionDecl);
  void dispatchVarDecl(const VarDecl *VarDecl);
  void dispatchUnaryOperator(const UnaryOperator *Op);
  void dispatchBinaryOperator(const BinaryOperator *Op);
  void dispatchCallExpr(const CallExpr *Expr);
  void dispatchReturnStatement(const ReturnStmt *Stmt);
  void dispatchSwitchStmt(const SwitchStmt *Stmt);
  void dispatchConditionalOperator(const ConditionalOperator *CondOp);

  TypeProxy visitExpr(const Expr *Expr);
  TypeProxy visitUnaryOperator(const UnaryOperator *UnaryOp);
  TypeProxy visitBinaryOperator(const BinaryOperator *BinaryOp);
  TypeProxy visitConditionalOperator(const ConditionalOperator *CondOp);
  TypeProxy visitExplicitCastExpr(const ExplicitCastExpr *Expr);

  void visitVarDecl(const VarDecl *VarDecl, const Expr *Expr);
  void visitScalarDefinition(const VarDecl *VarDecl, const Expr *Expr);
  void visitRecordDefinition(const RecordType *VarType, const Expr *Expr);
  void visitArrayDefinition(const ArrayType *VarType, const Expr *Expr);

  const SourceManager *SrcM;
  llvm::DenseMap<SourceLocation, StringRef> LocTypeMap;
  llvm::DenseMap<StringRef, StringRef> ValueTypeMap;
  llvm::DenseSet<StringRef> TypeSet;
  llvm::DenseSet<StringRef> BoolTypes;
  const FunctionDecl *CurrentFunction;
  llvm::SmallString<64> NameBuffer;
};

} // namespace clang::tidy::autosar

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_TYPEUSAGECHECK_H
