{
  "targets": [
    {
      "target_name": "pitft",
      "sources": [ "src/pitft.cc", "src/framebuffer.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
