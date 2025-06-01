rule Custom_Windows_Trojan_Cobaltstrike {
    meta:
        author = "xkarma98"
        description = "Identifies dll load module from Cobalt Strike"
        severity = 100
        arch_context = "x64"
        scan_context = "file, memory"
        os = "windows"

    strings:
        $a1 = "__imp_BeaconPrintf" ascii fullword
        $b1 = "__imp_ADVAPI32$RegOpenKeyExA" ascii fullword
        $b2 = "__imp_ADVAPI32$RegQueryValueExA" ascii fullword
        $b3 = "__imp_ADVAPI32$RegCloseKey" ascii fullword

        
    condition:
        1 of ($a*) or 2 of ($b*)
}
