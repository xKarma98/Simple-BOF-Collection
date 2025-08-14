rule Custom_Windows_Trojan_CobaltStrike {
    meta:
        author = "xkarma98"
        description = "Identifies DLL load module from Cobalt Strike"
        severity = "100"
        arch_context = "x64"
        scan_context = "file, memory"
        os = "windows"

    strings:
        $a1 = "__imp_BeaconPrintf" ascii fullword
        $b1 = "__imp_NETAPI32$NetServerEnum" ascii fullword

    condition:
        any of ($a*) or any of ($b*)
}
