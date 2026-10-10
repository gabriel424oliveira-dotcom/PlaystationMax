# PKG checklist

This is a checklist, not a build script. The exact packaging commands depend on the OpenOrbis release and the working sample used as a template.

- [ ] Confirm exact target firmware and homebrew compatibility.
- [ ] Install a published OpenOrbis toolchain release on a supported host.
- [ ] Build its untouched `hello_world` sample successfully.
- [ ] Build SDL-PS4 and any required SDL_ttf/font dependency for the same target.
- [ ] Compile and link PlaystationMax using the known-good sample's configuration.
- [ ] Confirm executable conversion produces the expected `eboot.bin`.
- [ ] Prepare consistent application metadata (`param.sfo`) and all required modules/assets.
- [ ] Generate the GP4/project data using the matching packaging tools.
- [ ] Create the `.pkg` using that release's documented workflow.
- [ ] Test install/launch in the intended homebrew environment.

A package being generated does not guarantee installability or runtime compatibility.
