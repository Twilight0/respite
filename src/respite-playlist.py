#!/usr/bin/env python3
"""Resolve a YouTube playlist URL into individual video URLs and titles."""

import json
import subprocess
import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: respite-playlist.py <playlist_url>", file=sys.stderr)
        sys.exit(1)

    url = sys.argv[1]

    try:
        result = subprocess.run(
            ["yt-dlp", "--no-check-certificates", "--flat-playlist", "-j", url],
            capture_output=True, text=True, timeout=60
        )
        if result.returncode != 0:
            print(f"yt-dlp failed: {result.stderr.strip()}", file=sys.stderr)
            sys.exit(1)
    except Exception as e:
        print(f"Failed to resolve playlist: {e}", file=sys.stderr)
        sys.exit(1)

    for line in result.stdout.strip().split("\n"):
        if not line:
            continue
        try:
            data = json.loads(line)
            video_url = data.get("url", "")
            title = data.get("title", "")
            if video_url:
                print(video_url)
                print(title)
        except json.JSONDecodeError:
            continue


if __name__ == "__main__":
    main()
