{
  "targets": [
    {
      "target_name": "nodetk",
      "sources": [ "nodetk.cc"],
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
