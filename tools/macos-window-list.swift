// JTDX-VU: list a PID's windows as "CGWindowID<TAB>title" (for screencapture -l) - swift tools/macos-window-list.swift <pid>
import CoreGraphics
import Foundation
let want = Int(CommandLine.arguments[1])!
let list = CGWindowListCopyWindowInfo([.optionAll], kCGNullWindowID) as! [[String: Any]]
for w in list { let pid = w["kCGWindowOwnerPID"] as? Int ?? 0; let name = w["kCGWindowName"] as? String ?? ""; let num = w["kCGWindowNumber"] as? Int ?? 0; if pid == want && !name.isEmpty { print("\(num)\t\(name)") } }
