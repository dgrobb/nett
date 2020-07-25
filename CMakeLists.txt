set(LLVM_LINK_COMPONENTS support)

add_clang_executable(nett
    Nett.cpp
    input/FileUtils.cpp
    input/FileInput.cpp
    output/OutputColors.cpp
    output/UserOutput.cpp
    violations/Violation.cpp
    violations/ViolationManager.cpp
    checks/utils/Tokens.cpp
    checks/utils/Typedef.cpp
    checks/naming/Hungarian.cpp
    checks/naming/NamingStyles.cpp
    checks/naming/VarNameCheck.cpp
    checks/naming/FuncNameCheck.cpp
    checks/naming/StructUnionNameCheck.cpp
    checks/naming/StructUnionFieldNameCheck.cpp
    checks/naming/EnumNameCheck.cpp
    checks/naming/EnumConstNameCheck.cpp
    checks/naming/DefineNameCheck.cpp
    checks/naming/TypedefNameCheck.cpp
    checks/comments/GlobalVarCommentCheck.cpp
    checks/comments/CommentManager.cpp
    checks/comments/FunctionCommentCheck.cpp
    checks/braces/BracesCheck.cpp
    checks/braces/FunctionBodyBracesCheck.cpp
    checks/braces/StructUnionBracesCheck.cpp
    checks/braces/EnumBracesCheck.cpp
    checks/whitespace/WhitespaceCheck.cpp
    checks/whitespace/PointerStyles.cpp
    checks/whitespace/BinaryOperatorWhitespaceCheck.cpp
    checks/whitespace/StructUnionMemberAccessWhitespaceCheck.cpp
    checks/whitespace/UnaryOperatorWhitespaceCheck.cpp
    checks/whitespace/StatementSemicolonWhitespaceCheck.cpp
    checks/whitespace/GlobalVariableSemicolonWhitespaceCheck.cpp
    checks/whitespace/FunctionBodyWhitespaceCheck.cpp
    checks/whitespace/EnumWhitespaceCheck.cpp
    checks/whitespace/StructUnionWhitespaceCheck.cpp
    checks/whitespace/FunctionCallWhitespaceCheck.cpp
    checks/whitespace/VarDeclWhitespaceCheck.cpp
    checks/whitespace/TernaryOperatorWhitespaceCheck.cpp
    checks/whitespace/FileContentManager.cpp
    checks/indentation/IndentCheck.cpp
    checks/indentation/GlobalVarIndentCheck.cpp
    checks/indentation/FuncDeclParamContIndentCheck.cpp
    checks/indentation/FunctionBodyIndentCheck.cpp
    checks/indentation/DefineIndentCheck.cpp
    checks/indentation/StructUnionBodyIndentCheck.cpp
    checks/indentation/EnumBodyIndentCheck.cpp
    checks/indentation/TypedefIndentCheck.cpp
    checks/overall/FuncDefinitionInHeaderCheck.cpp
    checks/overall/KeywordRedefCheck.cpp
    checks/overall/FuncLengthCheck.cpp
    checks/warning/GoToCheck.cpp
    )

set(GCC_COMPILE_FLAGS "-O3")
set(GCC_LINK_FLAGS "-static")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")

target_link_libraries(nett
    PRIVATE
    clangTooling
    clangBasic
    clangASTMatchers
    -static-libgcc
    -static-libstdc++
    )
