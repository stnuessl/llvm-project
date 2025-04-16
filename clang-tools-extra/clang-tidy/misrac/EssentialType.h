//===--- EssentialType.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_ESSENTIALTYPE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_ESSENTIALTYPE_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"

namespace clang::tidy::misrac {

class EssentialType {
public:
  static EssentialType createBoolean() { return EssentialType(ETC_BOOLEAN, 1); }
  static EssentialType createChar() { return EssentialType(ETC_CHARACTER, 8); }

  static EssentialType create(const CharacterLiteral *Expr) {
    if (Expr->getKind() == CharacterLiteralKind::Ascii)
      return EssentialType::createChar();

    return createUndefined();
  }

  static EssentialType create(const IntegerLiteral *Expr) {
    return create(Expr, Expr->getValue());
  }

  // FIXME: Does it make sense to make 'createUndefined()' public to avoid
  // code duplication just for the sake of encapsulation?
  // More general: How could I refactor the code to make the this class
  // more streamlined? Introduce a factory?
  static EssentialType create(const StringLiteral *Expr) {
    (void)Expr;

    return createUndefined();
  }

  static EssentialType create(const RecoveryExpr *Expr) {
    (void)Expr;

    return createUndefined();
  }

  static EssentialType create(const Expr *Expr, const llvm::APInt &Value) {
    auto Category = getCategory(Expr);
    uint32_t Size;

    if (Value.isNegative())
      Size = Value.getSignificantBits();
    else
      Size = Value.getActiveBits() + (Category == ETC_SIGNED);

    Size = getMinimumRequiredSize(Size);

    return EssentialType(Category, Size);
  }

  static EssentialType create(QualType Type, const TypeInfo &Info) {
    return create(Type, Info.Width);
  }

  static EssentialType create(const EnumDecl *Decl) {
    auto Name = Decl->getName();
    if (Name.empty()) {
      // The type of an enumerator belonging to an unnamed enum is defined to
      // be 'int', so we just need to figure out the minimum amount of bits
      // required to represent all of the the enum's values.
      auto N1 = Decl->getNumNegativeBits();
      auto N2 = Decl->getNumPositiveBits();
      auto Size = std::max(N1, N2);

      return EssentialType(ETC_SIGNED, getMinimumRequiredSize(Size));
    }

    uint32_t Hash = 0x9b05688c;

    for (auto C : Name) {
      Hash <<= 1;
      Hash ^= C;
    }

    for (const auto *It : Decl->enumerators()) {
      Hash <<= 1;
      Hash ^= It->getName().back();
    }

    return EssentialType(ETC_ENUM, Hash);
  }

  bool isValid() const { return Category != ETC_UNDEFINED; }
  bool isBoolean() const { return Category == ETC_BOOLEAN; }
  bool isChar() const { return Category == ETC_CHARACTER; }
  bool isEnum() const { return Category == ETC_ENUM; }
  bool isFloating() const { return Category == ETC_FLOATING; }
  bool isSigned() const { return Category == ETC_SIGNED; }
  bool isUnsigned() const { return Category == ETC_UNSIGNED; }

  bool isInteger() const { return isSigned() || isUnsigned(); }

  bool isSameCategory(const EssentialType &Other) const {
    return Category == Other.Category;
  }

  bool isWider(const EssentialType &Other) const {
    // Do not use on enums.
    return Data > Other.Data;
  }

  bool isNarrower(const EssentialType &Other) const {
    // Do not use on enums.
    return Data < Other.Data;
  }

  bool operator==(const EssentialType &Other) const {
    return std::tie(Category, Data) == std::tie(Other.Category, Other.Data);
  }

  bool operator!=(const EssentialType &Other) const {
    return std::tie(Category, Data) != std::tie(Other.Category, Other.Data);
  }

  bool isSameIntegerCategory(const EssentialType &Other) const {
    return isInteger() && Category == Other.Category;
  }

  bool hasAppropriateCastTo(const EssentialType &Other) {
    // Rule 10.5 - Amplification
    if (isSameCategory(Other))
      return !isEnum() || *this == Other;

    switch (Category) {
    case ETC_BOOLEAN:
      return false;
    case ETC_CHARACTER:
      return Other.isChar() || Other.isInteger();
    case ETC_ENUM:
      return Other.isChar() || Other.isInteger() || Other.isFloating();
    case ETC_FLOATING:
      return Other.isInteger();
    case ETC_SIGNED:
      return Other.isChar() || Other.isUnsigned() || Other.isFloating();
    case ETC_UNSIGNED:
      return Other.isChar() || Other.isSigned() || Other.isFloating();
    default:
      return false;
    }
  }

  StringRef getCategoryAsString() const {
    return getCategoryAsString(Category);
  }

  // Do not use on enums.
  uint32_t getSize() const { return Data; }

  friend const StreamingDiagnostic &operator<<(const StreamingDiagnostic &SD,
                                               const EssentialType ET) {
    SD << ET.getCategoryAsString();

    return SD;
  }

private:
  enum EssentialTypeCategory : uint8_t {
    ETC_UNDEFINED,
    ETC_BOOLEAN,
    ETC_CHARACTER,
    ETC_ENUM,
    ETC_FLOATING,
    ETC_SIGNED,
    ETC_UNSIGNED
  };

  static StringRef getCategoryAsString(EssentialTypeCategory Category) {
    switch (Category) {
    case ETC_BOOLEAN:
      return "boolean";
    case ETC_CHARACTER:
      return "character";
    case ETC_ENUM:
      return "enum";
    case ETC_FLOATING:
      return "floating";
    case ETC_SIGNED:
      return "signed";
    case ETC_UNSIGNED:
      return "unsigned";
    default:
      return "undefined";
    }
  }

  static EssentialTypeCategory getCategory(const BuiltinType *Type) {
    switch (Type->getKind()) {
    case BuiltinType::Bool:
      return ETC_BOOLEAN;
    case BuiltinType::Char_U:
    case BuiltinType::Char_S:
      return ETC_CHARACTER;
    case BuiltinType::Float:
    case BuiltinType::Double:
    case BuiltinType::LongDouble:
    case BuiltinType::Float128:
    case BuiltinType::Ibm128:
      return ETC_FLOATING;
    case BuiltinType::SChar:
    case BuiltinType::Short:
    case BuiltinType::Int:
    case BuiltinType::Long:
    case BuiltinType::LongLong:
    case BuiltinType::Int128:
      return ETC_SIGNED;
    case BuiltinType::UChar:
    case BuiltinType::UShort:
    case BuiltinType::UInt:
    case BuiltinType::ULong:
    case BuiltinType::ULongLong:
    case BuiltinType::UInt128:
      return ETC_UNSIGNED;
    default:
      return ETC_UNDEFINED;
    }
  }

  static EssentialTypeCategory getCategory(const Expr *Expr) {
    return getCategory(Expr->getType());
  }

  static EssentialTypeCategory getCategory(QualType Type) {
    if (const auto *BT = Type->getAs<BuiltinType>())
      return getCategory(BT);

    if (Type->isEnumeralType())
      return ETC_ENUM;

    return ETC_UNDEFINED;
  }

  static uint32_t getMinimumRequiredSize(uint32_t Size) {
    if (llvm::isPowerOf2_32(Size))
      return Size;

    return llvm::NextPowerOf2(Size);
  }

  static EssentialType create(const BuiltinType *Type, uint32_t Size) {
    return EssentialType(getCategory(Type), Size);
  }

  static EssentialType create(QualType Type, uint32_t Size) {
    if (const auto *BT = Type->getAs<BuiltinType>()) {
      auto Category = getCategory(BT);

      // Ensure to have a consistent size for booleans and characters.
      switch (Category) {
      case ETC_UNDEFINED:
        return createUndefined();
      case ETC_BOOLEAN:
        return createBoolean();
      case ETC_CHARACTER:
        return createChar();
      default:
        return EssentialType(Category, Size);
      }
    }

    if (const auto *ET = Type->getAs<EnumType>())
      return create(ET->getDecl());

    return createUndefined();
  }

  static EssentialType createUndefined() {
    return EssentialType(ETC_UNDEFINED, 0);
  }

  EssentialType(EssentialTypeCategory Category, uint32_t Data)
      : Category(Category), Data(Data) {}

  EssentialTypeCategory Category;
  uint32_t Data : 24;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_ESSENTIALTYPE_H
