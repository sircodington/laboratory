//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <cstdlib>
#include <iostream>
#include <string>

#include <core/BitField.h>
#include <core/Formatting.h>
#include <core/List.h>
#include <sigil/DfaScannerDriver.h>
#include <sigil/DfaTableScannerDriver.h>
#include <sigil/Grammar.h>

String range_string(const sigil::ScannerDriver::FileRange &range)
{
    StringBuilder b;
    Formatting::format_into(
        b,
        range.file_path,
        ":",
        range.first.line,
        ":",
        range.first.column,
        "-",
        range.end.line,
        ":",
        range.end.column);
    return b.to_string();
}

template<typename Scanner>
static void scanner_repl(const sigil::Grammar &grammar, Scanner &scanner)
{
    debug_log("> ");
    for (std::string std_line; std::getline(std::cin, std_line);) {
        const StringView line(std_line.data(), s64(std_line.size()));
        if (line.is_empty()) {
            debug_log("> ");
            continue;
        }
        if (line == "^D" or line == "^Z")
            break;

        scanner.initialize("<repl-string>", line);
        while (scanner.has_next()) {
            const auto token = scanner.next();
            const auto token_name = token.type == -2 ? "Eof"
                                    : token.type == -1
                                        ? "Error"
                                        : grammar.token_names()[token.type];

            debug_log(
                token_name,
                "(",
                range_string(token.range),
                ": ",
                token.lexeme,
                ")",
                "\n");
        }

        debug_log("> ");
    }
    debug_log("Done!\n");
}

int main()
{
    sigil::Specification spec;
    /* 0 */ spec.add_literal_token("Plus", "+");
    /* 1 */ spec.add_literal_token("Star", "*");
    /* 2 */ spec.add_literal_token("KwForeach", "foreach");
    /* 3 */ spec.add_literal_token("OpenParenthesis", "(");
    /* 4 */ spec.add_literal_token("CloseParenthesis", ")");
    /* 5 */ spec.add_regex_token("Literal", "[0-9]+");
    /* 6 */ spec.add_regex_token("KwIf", "if");
    /* 7 */ spec.add_regex_token("Identifier", "[a-zA-Z_][a-zA-Z0-9_]*");
    /* 8 */ spec.add_regex_token("Whitespace", "[ \\n\\r]+");

    for (const auto &token : spec.tokens()) { debug_log(token, "\n"); }

    auto either_grammar = sigil::Grammar::compile(spec);
    if (not either_grammar.isRight()) {
        debug_log("Error: ", either_grammar.left(), "\n");
        return EXIT_FAILURE;
    }

    const auto grammar = std::move(either_grammar.release_right());
    debug_log(grammar.dfa(), "\n");

    auto scanner =
        sigil::DfaTableScannerDriver::create(grammar.dfa());
    scanner_repl(grammar, scanner);

    return EXIT_SUCCESS;
}
