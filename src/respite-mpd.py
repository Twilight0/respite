#!/usr/bin/env python3
"""Construct a local MPD manifest from yt-dlp metadata for GStreamer DASH playback.
   For live streams with combined formats, outputs the direct URL instead."""

import html
import json
import os
import subprocess
import sys
import tempfile

def main():
    if len(sys.argv) < 2:
        print("Usage: respite-mpd.py <url>", file=sys.stderr)
        sys.exit(1)

    url = sys.argv[1]

    try:
        result = subprocess.run(
            ["yt-dlp", "--no-check-certificates", "-j", url],
            capture_output=True, text=True, timeout=30
        )
        if result.returncode != 0:
            print(f"yt-dlp failed: {result.stderr.strip()}", file=sys.stderr)
            sys.exit(1)

        data = json.loads(result.stdout)
    except Exception as e:
        print(f"Failed to get metadata: {e}", file=sys.stderr)
        sys.exit(1)

    title = data.get("title", "")
    is_live = data.get("is_live", False)

    # Find best video (mp4/dash) and best audio
    video_fmt = None
    audio_fmt = None

    for fmt in data.get("formats", []):
        fmt_id = fmt.get("format_id", "")
        ext = fmt.get("ext", "")
        vcodec = fmt.get("vcodec", "none")
        acodec = fmt.get("acodec", "none")

        # Skip format 18 (combined 360p) — we want DASH for higher quality
        if fmt_id == "18":
            continue

        # Video-only formats (prefer mp4)
        if vcodec != "none" and acodec == "none" and ext == "mp4":
            if video_fmt is None or fmt.get("height", 0) > video_fmt.get("height", 0):
                video_fmt = fmt

        # Audio-only formats (prefer m4a/mp4 for compatibility)
        if acodec != "none" and vcodec == "none":
            is_mp4 = ext in ("mp4", "m4a")
            if audio_fmt is None:
                audio_fmt = fmt
            elif is_mp4 and audio_fmt.get("ext") not in ("mp4", "m4a"):
                audio_fmt = fmt
            elif ext == audio_fmt.get("ext") and fmt.get("abr", 0) > audio_fmt.get("abr", 0):
                audio_fmt = fmt

    # If we have separate streams, build an MPD
    if video_fmt and audio_fmt:
        duration = int(data.get("duration", 0) or 0)
        video_url = html.escape(video_fmt["url"])
        audio_url = html.escape(audio_fmt["url"])
        width = video_fmt.get("width", 1920)
        height = video_fmt.get("height", 1080)
        video_br = int(video_fmt.get("vbr", 2000000)) * 1000
        audio_br = int(audio_fmt.get("abr", 128000)) * 1000
        audio_sr = int(audio_fmt.get("audio_sample_rate", 44100))
        framerate = video_fmt.get("fps", 30)

        video_ext = video_fmt.get("ext", "mp4")
        audio_ext = audio_fmt.get("ext", "mp4")
        video_mime = "video/mp4" if video_ext in ("mp4", "m4a") else "video/webm"
        audio_mime = "audio/mp4" if audio_ext in ("mp4", "m4a") else "audio/webm"

        mpd = f"""<?xml version="1.0" encoding="UTF-8"?>
<MPD xmlns="urn:mpeg:dash:schema:mpd:2011"
     type="static"
     mediaPresentationDuration="PT{duration}S"
     minBufferTime="PT2S">
  <Period>
    <AdaptationSet mimeType="{video_mime}">
      <Representation id="0" bandwidth="{video_br}" width="{width}" height="{height}" frameRate="{framerate}">
        <BaseURL>{video_url}</BaseURL>
      </Representation>
    </AdaptationSet>
    <AdaptationSet mimeType="{audio_mime}">
      <Representation id="1" bandwidth="{audio_br}" audioSamplingRate="{audio_sr}">
        <BaseURL>{audio_url}</BaseURL>
      </Representation>
    </AdaptationSet>
  </Period>
</MPD>"""

        fd, path = tempfile.mkstemp(suffix=".mpd", prefix="respite-")
        with os.fdopen(fd, "w") as f:
            f.write(mpd)

        print(f"file://{path}")
        print(title)
        return

    # No separate streams (live stream or fallback) — use best combined format
    best_combined = None
    for fmt in data.get("formats", []):
        vcodec = fmt.get("vcodec", "none")
        acodec = fmt.get("acodec", "none")
        ext = fmt.get("ext", "")
        if vcodec != "none" and acodec != "none" and ext == "mp4":
            if best_combined is None or fmt.get("height", 0) > best_combined.get("height", 0):
                best_combined = fmt

    # Last resort: any combined format
    if best_combined is None:
        for fmt in data.get("formats", []):
            vcodec = fmt.get("vcodec", "none")
            acodec = fmt.get("acodec", "none")
            if vcodec != "none" and acodec != "none":
                if best_combined is None or fmt.get("height", 0) > best_combined.get("height", 0):
                    best_combined = fmt

    if best_combined is None:
        print("No playable format found", file=sys.stderr)
        sys.exit(1)

    print(best_combined["url"])
    print(title)


if __name__ == "__main__":
    main()
