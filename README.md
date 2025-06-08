# Background Image Requirements

- The image should be at least **1920×1080 pixels**.
- It will be **cropped while preserving the aspect ratio**.

---

# PureMaps Setup

## Geoclue (for now)

### Installation

```bash
sudo apt-get install geoclue-2.0 geoclue-doc geoclue-2-demo
````

Then follow the instructions at:
[https://github.com/zeenix/gps-share](https://github.com/zeenix/gps-share)

---

### Configuration

Add the following to the end of `/etc/geoclue/geoclue.conf`:

```ini
[io.github.rinigus.PureMaps]
allowed=true
system=false
users=
```

---

### Reload the service


```bash
sudo systemctl daemon-reload && sudo systemctl restart geoclue.service
```


### Build
```bash
cmake .. \
  -DCMAKE_PREFIX_PATH=~/Qt/6.8.3/gcc_64

make -j$(nproc)
```

### Running

```bash
QT_XCB_GL_INTEGRATION=xcb_egl QT_WAYLAND_CLIENT_BUFFER_INTEGRATION=xcomposite-egl ./Car_Infotainment
```

```bash
QT_QPA_PLATFORM=wayland pure-maps
```
