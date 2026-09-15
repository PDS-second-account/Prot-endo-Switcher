#include "eg2/libs/typedefs.h"
#include "eg2/tex/textures.hh"
#include "eg2/tex/font.hh"
#include "eg2/tex/color.hh"
#include "eg2/render/screen.hh"
#include "eg2/controller/drc.hh"
#include "inkay/render/gui.hh"
#include "inkay/repos/versions.hh"
#include "inkay/repos/download.hh"
#include "inkay/filesystem/dirs.hh"
#include <memory>
#include <cstdio>

namespace Inkay {
    namespace GUI {
        static std::unique_ptr<Tex::LatteFont> Font;
        static std::unique_ptr<Tex::LatteColor> BackgroundFill;
        static std::unique_ptr<Tex::LatteColor> HeaderLine;
        static std::unique_ptr<Tex::LatteColor> PretendoCard;
        static std::unique_ptr<Tex::LatteColor> ProtariumCard;
        static std::unique_ptr<Tex::LatteColor> StatusPill;
        static std::unique_ptr<Tex::LatteTex> BrandSymbol;
        static std::unique_ptr<Tex::LatteTex> PretendoLogo;
        static std::unique_ptr<Tex::LatteTex> ProtariumLogo;

        static bool DidCenterFont = false;

        static const char* CurrentNetworkName(void) {
            switch (Inkay::Versions::FileState) {
                case Inkay::Versions::LocalFileState::ValidPretendo: return "PRETENDO";
                case Inkay::Versions::LocalFileState::ValidProtarium: return "PROTARIUM";
                case Inkay::Versions::LocalFileState::Missing: return "NO INKAY";
                default: return "UNKNOWN / CUSTOM";
            }
        }

        static u32 CurrentNetworkColor(void) {
            switch (Inkay::Versions::FileState) {
                case Inkay::Versions::LocalFileState::ValidPretendo: return Colors::Pretendo;
                case Inkay::Versions::LocalFileState::ValidProtarium: return Colors::Protarium;
                default: return Colors::Unknown;
            }
        }

        void Init(void) {
            Font = std::make_unique<Tex::LatteFont>(FONT_CAFESTD, 24.0f);
            BackgroundFill = std::make_unique<Tex::LatteColor>(Colors::Background);
            HeaderLine = std::make_unique<Tex::LatteColor>(Colors::Pretendo);
            PretendoCard = std::make_unique<Tex::LatteColor>(Colors::Surface);
            ProtariumCard = std::make_unique<Tex::LatteColor>(Colors::Surface);
            StatusPill = std::make_unique<Tex::LatteColor>(Colors::SurfaceAlt);

            BrandSymbol = std::make_unique<Tex::LatteTex>("fs:/vol/content/ui/brand-symbol.png");
            PretendoLogo = std::make_unique<Tex::LatteTex>("fs:/vol/content/ui/pretendo.png");
            ProtariumLogo = std::make_unique<Tex::LatteTex>("fs:/vol/content/ui/protarium.png");
        }

        void Render(void) {
            if (BackgroundFill) BackgroundFill->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 900.0f, 520.0f, 0.0f, 0.0f);

            const DownloadState state = Inkay::Download::State.load(std::memory_order_acquire);

            if (state == DOWNLOADSTATE_SELECT) {
                if (HeaderLine) {
                    HeaderLine->SetColor(CurrentNetworkColor());
                    HeaderLine->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 854.0f, 5.0f, 0.0f, -237.0f);
                }

                if (BrandSymbol) BrandSymbol->RenderTexCenteredScaled(RENDERRATIO_ASPECT_854x480, 112.0f, 112.0f, 0.0f, -156.0f);

                if (Font) {
                    Font->SetColor(Colors::White);
                    Font->RenderTextCenteredScaled(0.0f, -88.0f, RENDERRATIO_ASPECT_854x480, 470.0f, 55.0f, "Pret-arium Switcher");
                    Font->SetColor(Colors::Muted);
                    Font->RenderTextCenteredScaled(0.0f, -53.0f, RENDERRATIO_ASPECT_854x480, 520.0f, 40.0f, "Choose the Wii U network you want to use");
                }

                if (PretendoCard) PretendoCard->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 300.0f, 150.0f, -170.0f, 55.0f);
                if (ProtariumCard) ProtariumCard->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 300.0f, 150.0f, 170.0f, 55.0f);

                if (PretendoLogo) PretendoLogo->RenderTexCenteredScaled(RENDERRATIO_ASPECT_854x480, 82.0f, 82.0f, -170.0f, 25.0f);
                if (ProtariumLogo) ProtariumLogo->RenderTexCenteredScaled(RENDERRATIO_ASPECT_854x480, 82.0f, 82.0f, 170.0f, 25.0f);

                if (Font) {
                    Font->SetColor(Colors::Pretendo);
                    Font->RenderTextCenteredScaled(-170.0f, 112.0f, RENDERRATIO_ASPECT_854x480, 235.0f, 45.0f, "PRETENDO");
                    Font->SetColor(Colors::Protarium);
                    Font->RenderTextCenteredScaled(170.0f, 112.0f, RENDERRATIO_ASPECT_854x480, 235.0f, 45.0f, "PROTARIUM");
                }

                if (StatusPill) StatusPill->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 420.0f, 52.0f, 0.0f, 190.0f);
                if (Font) {
                    char status[96];
                    std::snprintf(status, sizeof(status), "Current network: %s", CurrentNetworkName());
                    Font->SetColor(CurrentNetworkColor());
                    Font->RenderTextCenteredScaled(0.0f, 190.0f, RENDERRATIO_ASPECT_854x480, 390.0f, 42.0f, status);
                }
            }
            else if (state == DOWNLOADSTATE_SELECT_ENV) {
                if (Font) {
                    Font->SetColor(Colors::White);
                    Font->RenderTextCenteredScaled(0.0f, -130.0f, RENDERRATIO_ASPECT_854x480, 520.0f, 60.0f, "Select installation environment");
                }

                const usize envCount = Inkay::Dirs::gEnvironments.size();
                if (envCount > 0 && Font) {
                    const usize selected = Inkay::Dirs::gSelectedEnv;
                    Font->SetColor(Colors::Muted);
                    Font->RenderTextCenteredScaled(0.0f, -25.0f, RENDERRATIO_ASPECT_854x480, 360.0f, 60.0f, Inkay::Dirs::gEnvironments[selected].c_str());
                    char counter[64];
                    std::snprintf(counter, sizeof(counter), "%d / %d", (int)(selected + 1), (int)envCount);
                    Font->RenderTextCenteredScaled(0.0f, 70.0f, RENDERRATIO_ASPECT_854x480, 120.0f, 42.0f, counter);
                    Font->RenderTextCenteredScaled(0.0f, 145.0f, RENDERRATIO_ASPECT_854x480, 340.0f, 42.0f, "D-Pad: choose   A: confirm");
                }
            }
            else if (state == DOWNLOADSTATE_DOWNLOADING) {
                if (Font) {
                    Font->SetColor(Colors::White);
                    Font->RenderTextCenteredScaled(0.0f, -20.0f, RENDERRATIO_ASPECT_854x480, 420.0f, 70.0f, "Downloading and installing Inkay...");
                    Font->SetColor(Colors::Muted);
                    Font->RenderTextCenteredScaled(0.0f, 45.0f, RENDERRATIO_ASPECT_854x480, 520.0f, 45.0f, "Do not remove the SD card");
                }
            }
            else if (state == DOWNLOADSTATE_ERROR) {
                if (Font) {
                    Font->SetColor(Colors::Protarium);
                    Font->RenderTextCenteredScaled(0.0f, -45.0f, RENDERRATIO_ASPECT_854x480, 620.0f, 70.0f, "Installation failed");
                    Font->SetColor(Colors::White);
                    Font->RenderTextCenteredScaled(0.0f, 30.0f, RENDERRATIO_ASPECT_854x480, 680.0f, 120.0f, Inkay::Download::LastError.c_str());
                }
            }
            else if (state == DOWNLOADSTATE_FINISHED) {
                if (Font) {
                    Font->SetColor(Colors::White);
                    Font->RenderTextCenteredScaled(0.0f, -35.0f, RENDERRATIO_ASPECT_854x480, 520.0f, 70.0f, "Inkay installed successfully");
                    Font->SetColor(Colors::Muted);
                    Font->RenderTextCenteredScaled(0.0f, 45.0f, RENDERRATIO_ASPECT_854x480, 420.0f, 50.0f, "Press B to restart the Wii U");
                }
            }
        }

        void Update(void) {
            Inkay::Download::UpdateDownloads();
            DRC::Update();

            if (Font && !DidCenterFont) {
                Font->SwitchRenderType(FONT_ALIGN_CENTER);
                DidCenterFont = true;
            }

            const DownloadState state = Inkay::Download::State.load(std::memory_order_acquire);
            if (state == DOWNLOADSTATE_SELECT) {
                // Left card: x=107..407, y=220..370
                if (DRC::IsTouchInsideSingle(107.0f, 220.0f, 300.0f, 150.0f)) {
                    Inkay::Download::PendingVersion = "Pretendo";
                    Inkay::Download::State.store(DOWNLOADSTATE_SELECT_ENV);
                    Inkay::Dirs::LoadEnvironments();
                }
                // Right card: x=447..747, y=220..370
                else if (DRC::IsTouchInsideSingle(447.0f, 220.0f, 300.0f, 150.0f)) {
                    Inkay::Download::PendingVersion = "Protarium";
                    Inkay::Download::State.store(DOWNLOADSTATE_SELECT_ENV);
                    Inkay::Dirs::LoadEnvironments();
                }
            }
            else if (state == DOWNLOADSTATE_SELECT_ENV) {
                const usize envCount = Inkay::Dirs::gEnvironments.size();
                if (envCount == 0) {
                    Inkay::Download::LastError = "No Aroma environment found";
                    Inkay::Download::State.store(DOWNLOADSTATE_ERROR);
                    return;
                }

                if (envCount > 1) {
                    if (DRC::ButtonTriggered(DRC::Input::Down)) Inkay::Dirs::gSelectedEnv = (Inkay::Dirs::gSelectedEnv + 1) % envCount;
                    if (DRC::ButtonTriggered(DRC::Input::Up)) Inkay::Dirs::gSelectedEnv = (Inkay::Dirs::gSelectedEnv + envCount - 1) % envCount;
                }

                if (envCount == 1 || DRC::ButtonTriggered(DRC::Input::A)) {
                    Inkay::Download::SelectedEnvironment = Inkay::Dirs::gEnvironments[Inkay::Dirs::gSelectedEnv];
                    if (Inkay::Download::PendingVersion == "Pretendo") Inkay::Download::PretendoDownload();
                    else Inkay::Download::ProtariumDownload();
                }
            }
            else if (state == DOWNLOADSTATE_FINISHED && DRC::ButtonTriggered(DRC::Input::B)) {
                Render::FullyRebootConsole();
            }
        }

        void Shutdown(void) {
            BrandSymbol.reset();
            PretendoLogo.reset();
            ProtariumLogo.reset();
            BackgroundFill.reset();
            HeaderLine.reset();
            PretendoCard.reset();
            ProtariumCard.reset();
            StatusPill.reset();
            Font.reset();
        }
    }
}
