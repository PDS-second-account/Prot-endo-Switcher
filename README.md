# Pret-arium Switcher

A Wii U Aroma utility for switching the installed Inkay build between **Pretendo** and **Protarium**.

Based on **Roséverse Installer** by Project Rosé (MIT license).

## What this patch changes

- Two-network UI: Pretendo / Protarium.
- Shows the currently detected network.
- Downloads the latest Pretendo Inkay release from `PretendoNetwork/Inkay`.
- Downloads the latest Protarium Inkay release from `Protarium-Network/Inkay-GitHub-Release`.
- Explicitly selects `Inkay-pretendo.zip` for Pretendo and `wiiu.zip` for Protarium.
- Writes the `.wms` and `.wps` into the selected Aroma environment using the original Roséverse Installer extraction system.
- Full reboot after installation.

## Apply to upstream

Copy these files over a clean checkout of `Project-Rose/Roseverse-Installer` and keep all unmodified `eg2/`, filesystem and certificate files from upstream.

Then build with devkitPro/devkitPPC + WUT using `make`.

## Credits

- Project Rosé & contributors — original Roséverse Installer.
- Pretendo Network — Inkay.
- Protarium Network — Protarium Inkay fork.
- Pret-arium Switcher adaptation for Wii U Aroma.
