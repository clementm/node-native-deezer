{
  "targets": [
    {
      "target_name": "addon",
      "variables": {
        "ARCH": "<!(node -p -e \"require('os').arch()\")",
        "dz_platform_dir": "<(module_root_dir)/deezer-sdk/Bins/Platforms"
      },
      "sources": [ "src/main.cc", "src/dzconnect/dzconnect.cc", "src/dzplayer/dzplayer.cc", "src/eventpool/eventpool.cc" ],
      "include_dirs": [ "<!(node -e \"require('nan')\")", "./deezer-sdk/Include" ],
      "conditions": [
        [ "OS == 'linux' and ARCH == 'x64'", {
          "libraries": [ "-ldeezer", "-Wl,-rpath <(dz_platform_dir)/Linux/x86_64" ]
        } ],
        [ "OS == 'linux' and ARCH == 'arm'", {
          "libraries": [ "-L<(dz_platform_dir)/Linux/arm", "-Wl,-rpath <(dz_platform_dir)/Linux/arm", "-ldeezer" ]
        } ]
      ]
    }
  ]
}