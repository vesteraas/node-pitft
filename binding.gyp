{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cc", "framebuffer.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
