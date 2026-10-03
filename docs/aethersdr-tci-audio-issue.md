# AetherSDR TCI audio bug - reported

Already tracked upstream as
[aethersdr/AetherSDR#6006](https://github.com/aethersdr/AetherSDR/issues/6006)
("TCI RX audio not restored when a TCI client reconnects (26.9.5
regression from 26.9.4)", priority high, diagnosed by the maintainers'
agent: the `dax|<channel>` RX binding is not rebuilt after the DAX stream
unregister - `refreshRxBindings()` missing in `onDaxStreamUnregistered()` /
`ensureDaxForTci()`). Not a new issue: Manoj's confirmation (FLEX-6600,
macOS 26, probe result) was posted there on 2026-10-03:
https://github.com/aethersdr/AetherSDR/issues/6006#issuecomment-5967032247

The draft written before #6006 was found is kept below for reference.

---

**Title:** TCI RX audio never resumes after the 10 s DAX release (audio_start acknowledged, no audio frames)

**AetherSDR:** 26.9.5, macOS 26 (Mac mini M4 Pro), FLEX-6600
**TCI client:** JTDX-VU 0.5.x (JTDX 2.2.159 TCI client), TCI audio on, receiver 0

**What happens**

1. A TCI client with audio running (`audio_start:0`) quits. JTDX sends
   `audio_stop:0` and closes the socket.
2. No other TCI client has audio, so `scheduleDaxRelease()` runs and DAX RX
   is released after `kDaxReleaseGraceMs` (10 s).
3. More than 10 s later (80 s in our case) a TCI client connects again and
   sends `audio_samplerate:48000;` and `audio_start:0;`.
4. AetherSDR answers `audio_start:0;` but **no binary audio frames are ever
   sent**. CAT over TCI works normally (vfo, modulation, rx_smeter).
5. Only restarting AetherSDR brings TCI audio back.

**Probe:** a minimal websocket client (`tools/tci_audio_probe.py`, no JTDX involved) connected to
`127.0.0.1:50001` while AetherSDR was in this state. It got the normal
start-up burst (`protocol:ExpertSDR3,1.5; ... audio_samplerate:48000;
audio_stream_sample_type:float32; audio_stream_channels:2; start; ready;`)
and 0 binary bytes in 2 s; then sent `audio_samplerate:48000;audio_start:0;`
and got the `audio_start:0;` echo with 0 binary bytes in the next 3 s.
Another TCI client (an Ulanzi deck plugin, no audio) stayed connected
throughout.

**Expected:** `audio_start` after a completed DAX release re-arms DAX
(`ensureDaxForTci()`) and audio frames resume, as they do when the client
comes back inside the 10 s grace window.

**Where it seems to go wrong:** `releaseDaxForTci()` releases the TCI
channel holds and sets `daxChannel(0)` on slices in `m_tciDaxSlices`;
`ensureDaxForTci()` then re-assigns a channel and calls
`acquireDaxChannel(ch, DaxConsumer::Tci)`, but no stream reaches the client.
Possibly the radio-side stream removal after the release grace window races
the re-acquire, or a slice whose DAX channel came from the profile is not
re-bound. Console logging ("TCI: releaseDaxForTci() releasing DAX RX",
"audio started for client") was not captured - happy to run with logging on
if you tell me how to get it into a file on macOS.

**Workaround:** restart AetherSDR. JTDX-VU 0.5.2 now reconnects TCI
automatically and warns when TCI audio stops arriving.
