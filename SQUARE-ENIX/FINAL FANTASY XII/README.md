Final Fantasy XII工具，其实这个有开坑，不过ms真坑了，到现在还沒啥动静


FF12 Tool v0.3 2008.5.3
Copyright (C) 2008, kid

FF12_Tool.exe [-dump|-rebuild|-import] [...] [isofile] [newisofile]
        -dump: [...]
                index                   Dump original index.
                txtindex                Dump index to txt.
                sorttxtindex            Dump index to txt. Sorted by LBA.
                file [...]
                        -all            Export all files.
                        -t num          Export all type(num) files.
                        -f expression   Export specific files.
                        -nodecode       Do not uncompress file.
        -rebuild: [...]
                -f folder       Specify import folder. Ignore to use default imp
ort folder 'files\'.

Example:
  FF12_Tool.exe -dump sorttxtindex ff12.iso
  FF12_Tool.exe -dump file -f 1:0-10,3:7,9:* ff12.iso
  FF12_Tool.exe -dump file -t 5 -nodecode ff12.iso
  FF12_Tool.exe -rebuild d:\ff12.iso c:\ff12_new.iso
