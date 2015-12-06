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
      ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_RTTI': 'YES',
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'MACOSX_DEPLOYMENT_TARGET':'10.8',
        'CLANG_CXX_LIBRARY': 'libc++',
        'CLANG_CXX_LANGUAGE_STANDARD':'c++11',
        'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0'
      },
    }
  ]
}
