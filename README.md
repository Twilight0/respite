# Respite Media Player

A modern simple media player based on GStreamer, forked from [Parole](https://gitlab.xfce.org/apps/parole).

## Features

- **Lightweight** — Minimal dependencies, fast startup
- **GTK3 native** — Integrates well with GTK3-based desktops (Cinnamon, MATE, XFCE)
- **GStreamer backend** — Plays virtually all media formats
- **Plugin system** — Extensible via MPRIS2, notifications, tray icon
- **Subtitle support** — Load and display subtitles
- **DVD/CD playback** — Direct disc playback support
- **Live streams** — Stream online content

## Dependencies

- GTK3 >= 3.24
- GStreamer >= 1.0
- GLib >= 2.66
- D-Bus
- TagLib

## Building

```bash
meson setup build
meson compile -C build
sudo meson install -C build
```

## Plugins

- **mpris2** — MPRIS2 D-Bus interface for media players
- **notify** — Desktop notifications
- **tray** — System tray integration

## Differences from Parole

- Removed Xfce-specific dependencies (libxfce4ui, libxfce4util, xfconf)
- Uses standard GTK3 widgets
- Independent from Xfce desktop environment
- Modernized codebase

## Original Source

This project is forked from Parole by the Xfce project:
- Original: https://gitlab.xfce.org/apps/parole
- License: GPL-2.0-or-later

## License

GPL-2.0-or-later
