begin(Spritesheet)
{
    .name       = "balloon_idle",
    .path       = "data/balloon_idle.bmp",
    .width      = 48,
    .height     = 48,
    .count      = 4,
    .frameskip  = 4
},
{
    .name       = "balloon_pop",
    .path       = "data/balloon_pop.bmp",
    .width      = 48,
    .height     = 48,
    .count      = 8,
    .frameskip  = 3, 
    .play_once  = true
},
{
    .name       = "enemybl_idle",
    .path       = "data/enemybl_idle.bmp",
    .width      = 48,
    .height     = 48,
    .count      = 4,
    .frameskip  = 4
},
{
    .name       = "enemybl_pop",
    .path       = "data/enemybl_pop.bmp",
    .width      = 48,
    .height     = 48,
    .count      = 8,
    .frameskip  = 3 
},
{
    .name       = "cac_idle",
    .path       = "data/cac_idle.bmp",
    .width      = 48,
    .height     = 48,
    .count      = 6,
    .frameskip  = 4
},
{
    .name       = "cac_fall",
    .path       = "data/cac_fall.bmp",
    .width      = 48,
    .height     = 48,
    .count      = 11,
    .frameskip  = 3
},
end(Spritesheet)

begin(Palette)
{
    .name = "gamepal",
    .path = "data/cac_idle.bmp" 
},
end(Palette)

begin(Texture)
{
    .name       = "cloud",
    .path       = "data/cloud.bmp",
    .width      = 128,
    .height     = 48
},
end(Texture)

begin(Package)
{
    .name = "main",
    .contents = (const char *[]){
        "gamepal",
        "cloud",
        "balloon_idle",
        "balloon_pop",
        "cac_idle",
        "cac_fall",
        0
    }
},
end(Package)

none(Tilemap)
none(Tileset)

