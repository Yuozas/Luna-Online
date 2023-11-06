del *.obj *.ilk *.pdb *.tlb *.tli *.tlh *.tmp *.rsp *.pch *.idb *.sbr *.map *.bsc *.exp *.dep /s
FOR /F delims^= %%A IN ('DIR/AD/B/S^|SORT/R') DO RD "%%A"
pause