// Post a real key press (keycode, optional shift|alt|cmd) to the frontmost app.
// Used with click.swift for the JTDX-VU GUI tests in HANDOVER (throwaway -r instances): build with
//   swiftc -O -o key tools/key.swift
// and run `key <keycode> [shift|alt|cmd]` only after checking the target PID is frontmost.
// macOS keycodes: F1 122, F2 120, F3 99, F4 118, F5 96, F6 97, F7 98, F8 100, "," 43.
import Foundation
import CoreGraphics
let a = CommandLine.arguments
let code = CGKeyCode(Int(a[1])!)
var flags: CGEventFlags = []
if a.count > 2 { if a[2] == "shift" { flags = .maskShift } else if a[2] == "alt" { flags = .maskAlternate } else if a[2] == "cmd" { flags = .maskCommand } }
for down in [true, false] {
  let e = CGEvent(keyboardEventSource: nil, virtualKey: code, keyDown: down)!
  e.flags = flags
  e.post(tap: .cghidEventTap)
  usleep(50000)
}
