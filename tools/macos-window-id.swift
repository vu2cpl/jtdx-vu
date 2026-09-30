// JTDX-VU: print the CGWindowIDs of a PID's main windows (for screencapture -l) - swift tools/macos-window-id.swift <pid>
import CoreGraphics
import Foundation
let want = Int(CommandLine.arguments[1])!
let list = CGWindowListCopyWindowInfo([.optionAll], kCGNullWindowID) as! [[String: Any]]
for w in list { let pid = w["kCGWindowOwnerPID"] as? Int ?? 0; let name = w["kCGWindowName"] as? String ?? ""; let num = w["kCGWindowNumber"] as? Int ?? 0; if pid == want && name.contains("VUCG") { print(num) } }
