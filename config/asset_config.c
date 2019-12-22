begin(Palette)
{
    .name = "gamepal",
    .path = "data/CACP.BMP" 
},
end(Palette)

begin(Package)
{
    .name = "main",
    .contents = (const char *[]){
        "gamepal",
        0
    }
},
end(Package)

none(Texture)
none(Spritesheet)
none(Tilemap)
none(Tileset)

