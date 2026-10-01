// Post a real left click (or N clicks, for a double-click) at screen point x y.
// Used by the JTDX-VU GUI tests in HANDOVER (throwaway -r instances): build with
//   swiftc -O -o click tools/click.swift
// and run `click <x> <y> [count]` only after checking the target PID is frontmost.
import Foundation
import CoreGraphics
let a = CommandLine.arguments
let p = CGPoint(x: Double(a[1])!, y: Double(a[2])!)
let n = a.count > 3 ? Int(a[3])! : 1
CGEvent(mouseEventSource: nil, mouseType: .mouseMoved, mouseCursorPosition: p, mouseButton: .left)!.post(tap: .cghidEventTap)
usleep(100000)
for i in 1...n {
  let d = CGEvent(mouseEventSource: nil, mouseType: .leftMouseDown, mouseCursorPosition: p, mouseButton: .left)!
  d.setIntegerValueField(.mouseEventClickState, value: Int64(i)); d.post(tap: .cghidEventTap)
  usleep(30000)
  let u = CGEvent(mouseEventSource: nil, mouseType: .leftMouseUp, mouseCursorPosition: p, mouseButton: .left)!
  u.setIntegerValueField(.mouseEventClickState, value: Int64(i)); u.post(tap: .cghidEventTap)
  usleep(80000)
}
