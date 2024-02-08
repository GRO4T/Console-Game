#include "assets.h"

#include <fstream>

namespace ascii_combat {

// TODO: write unit tests and refactor this
void Assets::Load(const std::string& assets_filename) {
    std::ifstream assets_file;
    assets_file.open(assets_filename);

    string row = "";
    do {
        getline(assets_file, row);
        if (row != "EOF") {
            if (row == "Player") {
                for (int i = 0; i < ANIM_NUM; ++i) {
                    vector<Clip> anim;
                    while (1) {
                        getline(assets_file, row);
                        if (row == "EOA")  // End Of Animation
                            break;
                        else {
                            Clip clip(stoi(row));
                            while (1) {
                                vector<string> frame;
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
                                    clip.frames.emplace_back(frame);
                                    ++(clip.len);
                                }
                            }
                            anim.emplace_back(clip);
                        }
                    }
                    player_animations_.emplace_back(anim);
                }
            } else if (row == "Map") {
                vector<string> map;
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

const PlayerAnimations& Assets::GetPlayerAnimations() const {
    return player_animations_;
}

const std::vector<Map>& Assets::GetMaps() const { return maps_; }

}  // namespace ascii_combat
