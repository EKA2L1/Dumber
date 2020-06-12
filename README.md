# Dumber
Z: drive repackager for Symbian platform, compatible for all Symbian platform.

## Instructions:
* Please jailbreak your Symbian phone first. 
* After that, open Dumberdore in your menu, and choose "Dump RPKG" in Options menu.

## Specification:
* EKA2L1 uses the RPKG format to package all *Z:* drive contents. Z: drive should have at least ROM (on previous platform), or is mounted with both *ROFS + ROM*. All contents are repackage to a single file without any compression. All entries should be sort by UID.
* Header
   - Magic: char[4] (RPKG or RPK2 for 2nd version)
   - Version: 
      + Major: char
      + Minor: char
      + Build: short
   - File count: uint32_t
   - Header size (RPKG 2nd version): uint32_t
   - Machine UID (RPKG 2nd version): uint32_t
* Entry sections: *File count* * entries
   - Entry:
      + Attribute: Uint64_t
      + Time: TTime
      + FullPathLength: Uint64_t
      + PathStringRawData: UCS-2 string with length declared above
      + DataSize: Uint64_t
