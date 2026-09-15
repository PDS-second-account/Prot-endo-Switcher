#pragma once

#include "eg2/libs/typedefs.h"

namespace Inkay {
    namespace GUI {
        enum Colors : u32 {
            Background = 0x0B0D18FF,
            Surface = 0x171B2CFF,
            SurfaceAlt = 0x20263CFF,
            Pretendo = 0x9C63F5FF,
            Protarium = 0xE43D40FF,
            White = 0xFFFFFFFF,
            Muted = 0xB9C0D4FF,
            Unknown = 0x697187FF,
            Black = 0x000000FF
        };

        void Init(void);
        void Render(void);
        void Update(void);
        void Shutdown(void);
    }
}
