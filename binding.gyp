{
  "targets": [
    {
      "target_name": "pitft",
      "conditions": [
        ['OS=="linux"', {
          "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "<!@(pkg-config cairo --cflags-only-I | sed s/-I//g)"
          ],
          "sources": [ "src/pitft.cc", "src/framebuffer.cc" ],
          "libraries": [
            "<!@(pkg-config cairo --libs)"
          ]
        }]
        ]
    }
  ]
}
