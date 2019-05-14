#  這個 repo 提供一個遊戲框架
 - 可以直接由 Emscripten 編譯
 - 也有提供 cmake ，可以直接用 `g++` 或 `clang++` 來編譯（`C++17` 以上需要）
 - 這是我從零開始土炮出來的，結構相對簡單。需要效能的話，請去找 [Unreal engine](https://github.com/EpicGames) 
 
# 怎麼編譯？
 1. Emscripten
    ```
    # 首先裝好 Emscripten
    
    # compile
    make em
    
    # compile and run
    make emrun
    
    # compile and run in debug mode
    make emrun-debug
    ```
    
 2. Native
    ```
    # 首先裝好 SDL2, SDL2-ttf, SDL2-image, SDL2-net, cmake, make
    # 確認你的 compiler 支援 c++17
    
    # compile
    make cmake
    
    # compile and run
    make cmakerun
    
    # compile and run in debug mode
    make cmakerun-debug
    ```
    
# 框架簡介
    ```
    app: 管理 game loop
        1. event queue => 丟到 theme->on_event();
        2. calculate() 
            1. for all element calculate()
            2. handle for all collisions
        3. render();
    
    fps: 管理遊戲速度的，每秒更新一次 speed_factor_, fps
    
    theme: 畫面管理者，要配合 asset/theme/*.toml
        1. asset/theme/*.toml => 定義所有 element, camera, map
            1. camera    => 顯示地圖的區塊
            2. area      => 地圖
                1. .area => 地圖本身
                         => 第一行：tile.png
                         => 第二行：地圖大小 [寬 長]
                         => 列出所有的 .map
                2. .map  => 定義一小塊地圖
                         => 格式：`%d:%d` ： `hardness:tile number to show` ： 
                            例如：`1:20`  ： `不能通過：顯示 tile 20 號`
                         => 總共一定要九個（這個硬刻的，我很想改掉，但還沒弄）
            3. elements  => 元素（下面說明）
    
    element: 畫面裡面的東西，例如 player, ball, 球竿 
    裡面有許多東西，我只挑重點
    包含：
        1. name (std::string) => name
        2. anime_info_ (std::unique_ptr<animation>) => 管理 animation
        3. src_   (SDL_Rect)  => 圖片資訊
        4. valid_   (bool)    => 需不需要考慮碰撞他
        5. col_offset_ (pixel)=> 碰撞考慮，是由圖片外圍縮進 col_offset_ 個 pixel
        6. z_index_ (int)     => 繪畫順序
        7. type_    (type)    => element type, bit 上要分開
        8. hardness_(hardness)=> 硬度，硬的可以衝過軟的，軟的不能衝過硬的
        9. flag_    (flag)    => 有無重力，鬼，只跟地圖有關係，等等都可以設定
        10.bounce_direction   => 反彈方向。一個 element 受到撞擊的時候，是要要求對方往哪裡彈（抱歉，超簡易物理引擎）
        11.state_   (state)   => 速度、加速度
            1. dest_ (rect<>) => 在地圖上位置、大小
        12.collision          => 碰撞事件
            1. queue          => *global* queue for collisions. 這個地方我以後想改掉
    
    animation: 動畫管理器。像是管理 yoshi 的走動
    
    element_effect: element 的可算式動畫，例如放大、消失
        1. virtual on_start();
        2. virtual operator();
        這些都可以 override
    ```        
其他是輔助的檔案，有需要可以參考

TODO:
    以後會有一些重大改變
    
    1. 我最近才知道這個傢伙 [Non-Virtua Interfacel](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-Virtual_Interface)，近期會把整個 code 在修一遍
    
    2. data member 的命名系統不一致，以後全部都會改成後綴 `_` 我也要哪天來改個
    3. Document 在寫好一點
    4. 地圖的限制
    
# 有任何想要叫我重寫的地方，請發個 ISSUE
