


export function tokenise(str) {
    const stopWords = new Set([
        "and",
        "or",
        "the",
        "with",
        "of",
        "a",
        "an"
    ]);

    return [
        ...new Set(
            str
                .toLowerCase()
                .replace(/[^a-z0-9\s]/g, " ")
                .split(/\s+/)
                .filter(token =>
                    token.length > 0 &&
                    !stopWords.has(token)
                )
        )
    ];
}