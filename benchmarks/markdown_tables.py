from __future__ import annotations


def build_markdown_table(headers: list[str], rows: list[list[str]]) -> list[str]:
    n_cols = len(headers)
    widths = [len(h) for h in headers]

    normalized_rows: list[list[str]] = []
    for row in rows:
        cells = [str(row[i]) if i < len(row) else "" for i in range(n_cols)]
        normalized_rows.append(cells)
        for i, cell in enumerate(cells):
            widths[i] = max(widths[i], len(cell))

    header_line = "| " + " | ".join(
        headers[i].ljust(widths[i]) for i in range(n_cols)
    ) + " |"
    separator_line = "| " + " | ".join("-" * widths[i] for i in range(n_cols)) + " |"

    data_lines = [
        "| " + " | ".join(row[i].ljust(widths[i]) for i in range(n_cols)) + " |"
        for row in normalized_rows
    ]

    return [header_line, separator_line, *data_lines]
