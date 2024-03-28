{
  "targets": [
    {
      "target_name": "Unsafe",
      "sources": [ "unsafe.cc" ],
      "cflags": [ "-std=c++11" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-rtti" ]
    }
  ]
}
