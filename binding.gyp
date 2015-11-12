{
  "targets": [
    {
      "target_name": "hello2",
      "sources": [ "hello2.cc"],
      "libraries": [
          "-ltcl",
          "-ltk"
      ],
      "include_dirs": [
        "/Library/Frameworks/Tcl.framework/Headers/",
        "/opt/X11/include/",
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
