#!/usr/bin/env bash
# generate_history.sh — Extract all git data needed for docs/history.md
# Re-runnable: safe to execute multiple times.
# Output directory: /tmp/strata_git_data/

set -euo pipefail

OUTDIR="/tmp/strata_git_data"
mkdir -p "$OUTDIR"

echo "=== Extracting git data to $OUTDIR ==="

# Full commit log with stats
git log --pretty=format:'COMMIT|%H|%h|%ai|%an|%s' --stat --no-merges \
    > "$OUTDIR/git_full_log.txt"

# Condensed one-liner log
git log --pretty=format:'%ai|%h|%s' --no-merges \
    > "$OUTDIR/git_oneline.txt"

# Tag history (milestones/releases)
git tag -l --sort=-version:refname \
    --format='%(refname:short)|%(creatordate:iso)|%(subject)' \
    > "$OUTDIR/git_tags.txt"

# File change frequency
git log --pretty=format: --name-only --no-merges \
    | sort | uniq -c | sort -rn | head -50 \
    > "$OUTDIR/git_hotfiles.txt"

# Lines added/removed per author
git log --pretty=format:'%an' --numstat --no-merges \
    | awk 'NF==1{author=$0} NF==3{added[author]+=$1; removed[author]+=$2} END{for(a in added) print a"|"added[a]"|"removed[a]}' \
    > "$OUTDIR/git_author_stats.txt"

# Commit count by month
git log --pretty=format:'%ai' --no-merges \
    | cut -d'-' -f1,2 | sort | uniq -c \
    > "$OUTDIR/git_monthly.txt"

# Commits with file lists (for phase detection)
git log --pretty=format:'%ai|%h|%s' --name-only --no-merges \
    > "$OUTDIR/git_commits_with_files.txt"

# Summary stats
{
    echo "First commit: $(git log --reverse --pretty=format:'%ai' | head -1)"
    echo "Last commit: $(git log --pretty=format:'%ai' | head -1)"
    echo "Total commits: $(git rev-list --count HEAD)"
    echo "Current HEAD: $(git rev-parse --short HEAD)"
    echo "Lines of code (src+include): $(find src/ include/ -name '*.cpp' -o -name '*.hpp' -o -name '*.h' 2>/dev/null | xargs wc -l 2>/dev/null | tail -1 | awk '{print $1}')"
    echo "Lines of tests: $(find tests/ -name '*.cpp' -o -name '*.py' 2>/dev/null | xargs wc -l 2>/dev/null | tail -1 | awk '{print $1}')"
    echo "Doc pages: $(find docs/ -name '*.md' 2>/dev/null | wc -l | tr -d ' ')"
} > "$OUTDIR/git_summary.txt"

echo "=== Done. Files written to $OUTDIR ==="
ls -la "$OUTDIR"
