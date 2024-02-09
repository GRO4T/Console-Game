#include "assets.h"

#include <fstream>

#include "config.h"

namespace ascii_combat {

// TODO: write unit tests and refactor this
Assets::Assets() {
    std::ifstream assets_file;
    assets_file.open(kAssetsFileName);

    std::string row = "";
    do {
        getline(assets_file, row);
        if (row != "EOF") {
            if (row == "Player") {
                for (int i = 0; i < ANIM_NUM; ++i) {
                    std::vector<Clip> anim;
                    while (1) {
                        getline(assets_file, row);
                        if (row == "EOA")  // End Of Animation
                            break;
                        else {
                            int ticks_per_frame = stoi(row);
                            std::vector<Frame> frames;
                            while (1) {
                                Frame frame;
                                getline(assets_file, row);
                                if (row == "EOC")  // End Of Clip
                                    break;
                                else {
                                    frame.emplace_back(row);
                                    while (1) {
                                        getline(assets_file, row);
                                        if (row == "EFM")  // End Of Frame
                                            break;
                                        else {
                                            frame.emplace_back(row);
                                        }
                                    }
                                    frames.emplace_back(frame);
                                }
                            }
                            anim.emplace_back(Clip(frames, ticks_per_frame));
                        }
                    }
                    player_animations_.emplace_back(anim);
                }
            } else if (row == "Map") {
                std::vector<std::string> map;
                while (1) {
                    getline(assets_file, row);
                    if (row == "EOM")
                        break;
                    else
                        map.emplace_back(row);
                }
                maps_.emplace_back(map);
            }
        }

    } while (row != "EOF");
    assets_file.close();
}

Assets& Assets::Instance() {
    static Assets assets;
    return assets;
}

const PlayerAnimations& Assets::GetPlayerAnimations() const { return player_animations_; }

const std::vector<Map>& Assets::GetMaps() const { return maps_; }

}  // namespace ascii_combat
