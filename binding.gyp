{
  "targets": [
    {
      "target_name": "pitft",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "<!@(pkg-config cairo --cflags-only-I | sed s/-I//g)"
      ],
      "sources": [ "src/pitft.cc", "src/framebuffer.cc" ],
      "conditions": [
        ['OS=="linux"', {
          "libraries": [
            "<!@(pkg-config cairo --libs)"
          ],
                    "include_dirs": [
                      "<!@(pkg-config cairo --cflags-only-I | sed s/-I//g)"
                    ]
        }]
        ]
    }
  ]
}
