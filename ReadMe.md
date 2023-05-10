## THƯ VIỆN SDL VÀ CÁCH CÀI
- Các thư viện SDL2 sử dụng : SDL.h, SDL_image.h, SDL_mixer.h, SDL_TTF.h
- Hướng dẫn cài các thư viện SDL : Tham khảo trên trang web Lazyfoo https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/codeblocks/index.php 
# MÔ TẢ TRÒ CHƠI
- Đây là một trò chơi điện tử phổ biến, thường được chơi trên các thiết bị di động hoặc máy tính.
- Trong trò chơi này, người chơi phải điều khiển một nhân vật (thường là một con vật hoặc một nhân vật đơn giản) để vượt qua các chướng ngại vật xuất hiện trên màn hình.
![image](https://github.com/ltl2702/PROJECT/assets/125105532/fb42f142-15bc-4200-abb4-50dbf0ea818b)

# LUẬT CHƠI
-  Người chơi thua cuộc khi rơi vào hố, hết thời gian hoặc điểm số của họ nhỏ hơn 0. 
-  Khi kết thúc bản đồ và thu thập kho báu, người chơi chiến thắng và trò chơi kết thúc.
# CÁCH CHƠI
-  Trong trò chơi, người chơi sử dụng các phím mũi tên trái, phải và lên để di chuyển nhân vật của mình.
-  Khi chạm vào một đồng xu, người chơi sẽ được cộng thêm 1 điểm. Nếu chạm vào một quả bom, người chơi sẽ mất 10 điểm. 
# TÍNH NĂNG
- Chương trình sử dụng thư viện SDL để hiển thị hình ảnh, text và âm thanh. 
![image](https://github.com/ltl2702/PROJECT/assets/125105532/ee42f699-9fca-49a9-8275-2f7d9f91b897)

- Các đối tượng được tạo ra trong trò chơi gồm: background, game map, nhân vật, tiền và thời gian. 
- Chương trình sử dụng một vòng lặp game để xử lý tất cả các hoạt động trong trò chơi và hiển thị trạng thái mới nhất của trò chơi.
- Trong mỗi lần lặp lại, chương trình sẽ xử lý các sự kiện SDL, di chuyển nhân vật và cập nhật các đối tượng khác trên màn hình.
