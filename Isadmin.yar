rule Custom_Windows_Trojan_Cobaltstrike {
    meta:
        author = "xkarma98"
        description = "Identifies dll load module from Cobalt Strike, checking for admin permission."
        severity = 100
        arch_context = "x64"
        scan_context = "file, memory"
        os = "windows"

    strings:
        $a1 = "__imp_BeaconPrintf" ascii fullword
        $a2 = "We are admin let's gooooo" ascii fullword
        

        
    condition:
        any of ($a*)
}
