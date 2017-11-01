#include "matchers.h"

using namespace clang;
using namespace clang::ast_matchers;

StatementMatcher intAssignmentMatcher =
		binaryOperator(
				hasOperatorName("="),
				hasLHS(
						anyOf(
							declRefExpr(
								to(
									varDecl(
										hasType(isInteger())
									).bind("varDecl"))),
							memberExpr(
								member(
									hasType(
										(TypeMatcher) anyOf(
											isInteger(),
											referenceType(pointee(isInteger())))))).bind("member")))).bind("intAssignment");

StatementMatcher intAssignmentOverloadedMatcher =
		cxxOperatorCallExpr(
			hasOverloadedOperatorName("="),
			hasArgument(0,
				anyOf(
					memberExpr(
						member(
							hasType(
								cxxRecordDecl(
									(internal::Matcher<CXXRecordDecl>) anyOf(
										isSameOrDerivedFrom("sc_port"),
										isSameOrDerivedFrom("sc_signal")))))).bind("member"),
					declRefExpr(to(varDecl(
						hasType(
							cxxRecordDecl(
									(internal::Matcher<CXXRecordDecl>) anyOf(
										isSameOrDerivedFrom("sc_port"),
										isSameOrDerivedFrom("sc_signal"))))).bind("varDecl")))))).bind("intAssignmentOverloaded");

StatementMatcher intAssignmentMatcherNonNull =
		binaryOperator(
				hasOperatorName("="),
				hasLHS(
						anyOf(
							declRefExpr(
								to(
									varDecl(
										hasType(isInteger())
									).bind("varDecl"))),
							memberExpr(
								member(
									hasType(
										(TypeMatcher) anyOf(
											isInteger(),
											referenceType(pointee(isInteger())))))).bind("member"))),
				unless(hasRHS(
					integerLiteral(equals(0))))
				).bind("intAssignment");

StatementMatcher intAssignmentOverloadedMatcherNonNull =
		cxxOperatorCallExpr(
			hasOverloadedOperatorName("="),
			hasArgument(0,
				anyOf(
					memberExpr(
						member(
							hasType(
								cxxRecordDecl(
									(internal::Matcher<CXXRecordDecl>) anyOf(
										isSameOrDerivedFrom("sc_port"),
										isSameOrDerivedFrom("sc_signal")))))).bind("member"),
					declRefExpr(to(varDecl(
						hasType(
							cxxRecordDecl(
									(internal::Matcher<CXXRecordDecl>) anyOf(
										isSameOrDerivedFrom("sc_port"),
										isSameOrDerivedFrom("sc_signal"))))).bind("varDecl"))))),
			unless(hasArgument(1,integerLiteral(equals(0))))
				).bind("intAssignmentOverloaded");

StatementMatcher sensitiveMatcher =
		cxxOperatorCallExpr(
			hasArgument(0,memberExpr(
					hasDeclaration(namedDecl(hasName("sensitive"))))),
			unless(hasArgument(1,
					cxxBindTemporaryExpr(
							has(cxxConstructExpr(
									has(declRefExpr(
											to(varDecl(hasName("update_handle")))))))))),
			hasOverloadedOperatorName("<<")
		).bind("sensitive");

StatementMatcher connectPortMatcher =
		cxxOperatorCallExpr(
			hasOverloadedOperatorName("()"),
			hasArgument(0,
				memberExpr(
						hasObjectExpression(
							hasType(
								cxxRecordDecl(isSameOrDerivedFrom("sc_module"))))
					)),
			hasArgument(1,
				declRefExpr(
					to(
						varDecl(
							hasType(
									classTemplateSpecializationDecl()),
							hasType(
								cxxRecordDecl(isSameOrDerivedFrom("sc_signal")))).bind("varDecl"))))
		).bind("connectPorts");

DeclarationMatcher signalVarDeclMatcher =
		varDecl(
			hasType(
				cxxRecordDecl(isSameOrDerivedFrom("sc_signal"))),
			hasType(
				classTemplateSpecializationDecl(
					))).bind("signalVarDecl");

clang::ast_matchers::StatementMatcher getConnectPortMatcherWithSignalName(const std::string& signalName) {
	StatementMatcher namedConnectPortMatcher =
			cxxOperatorCallExpr(
				hasOverloadedOperatorName("()"),
				hasArgument(0,
					memberExpr(
							hasObjectExpression(
								hasType(
									cxxRecordDecl(isSameOrDerivedFrom("sc_module"))))
						)),
				hasArgument(1,
					declRefExpr(
						to(
							varDecl(
								hasName(signalName),
								hasType(
									cxxRecordDecl(isSameOrDerivedFrom("sc_signal")))).bind("varDecl"))))
			).bind("connectPorts");

	return namedConnectPortMatcher;
}

clang::ast_matchers::StatementMatcher ifStmtMatcher =
		ifStmt().bind("if");

clang::ast_matchers::StatementMatcher whileStmtMatcher =
		whileStmt().bind("while");

StatementMatcher floatLiteralMatcher =
		floatLiteral().bind("float");

DeclarationMatcher signalFieldDeclMatcher =
		fieldDecl(
			hasType(
				cxxRecordDecl(
					isSameOrDerivedFrom("sc_signal"))),
			hasType(
				classTemplateSpecializationDecl(
					hasTemplateArgument(0,
						refersToType(isInteger()))))).bind("field");

