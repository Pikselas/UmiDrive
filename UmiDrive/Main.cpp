
#include<memory>
#include<thread>

#include "Spike/HttpServer.h"

#include"Label.h"
#include "TextEntry.h"
#include"TextButton.h"
#include "ImageButton.h"
#include"ScrollWindow.h"
#include "StandardWindow.h"

#include "DriveRuntime.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{

    HttpServer server("3456");
    std::thread([&]()
        {
           server.Serve();
        }).detach();

    StandardWindow window("UmiDrive" , 850);
    
    TextEntry token_entry(window , 5 , 10 , 150 , 30 , "Enter Token Here...");
    TextEntry email_entry(window , 5 , 50 , 150 , 30 , "Enter Email Here...");
    TextButton login_button(window ,"Login" , 5, 90, 150, 30);

    ScrollVerticalWindow contents_window(window , 170 , 10 , 660 , 550 , 800);
    const auto img = LoadImage(NULL, "D:/SeqDownLogo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    std::vector<std::unique_ptr<ImageButton>> buttons;

    login_button.OnClick = [&](auto&)
    {
        const auto token_str = token_entry.GetText();
        const auto email_str = email_entry.GetText();
        
        auto ID = DriveRuntime::LoadDrive(token_str.c_str(), token_str.length(), email_str.c_str(), email_str.length());
        if (ID != -1)
        {
            buttons.clear();
            int x = 5;
            int y = 5;
            std::function<void(std::string)> add_window = [& , ID](std::string name)
            {
                buttons.emplace_back(std::make_unique<ImageButton>(contents_window, name, img, x, y, 150, 150, [name , server = &server , ID](auto& wnd)
                    {
                        MessageBox(NULL, name.c_str(), "Entry created", MB_OK);
                        server->OnPath("/" + name, [ID , name](auto& req, Response& resp)
                            {
                                auto n_cstr = name.c_str();
                                auto n_len = name.length();
                                auto File = DriveRuntime::LoadFile(ID, name.c_str(), name.length());
                                if (File.ID != -1)
                                {
                                    resp.HEADERS.Set("Content-Length", std::to_string(File.Size));
                                    struct FileCloser
                                    {
                                        int DriveID;
                                        int FileID;
                                        std::string name;
                                        ~FileCloser()
                                        {
											DriveRuntime::CloseFile(DriveID , FileID);
                                            OutputDebugStringA("\n\nFile Closed\n\n");
                                            OutputDebugStringA(name.c_str());
										}
                                    };
                                    resp.SendRaw([ID, File, Closer = std::make_shared<FileCloser>( ID , File.ID , name )](std::span<char> buffer) ->std::optional<unsigned int>
                                        {

                                            auto size = DriveRuntime::ReadLoadedFile(ID, File.ID, buffer.data(), buffer.size());
                                            if (size != -1)
                                            {
                                                return size;
                                            }
                                            return std::nullopt;
                                        });
                                }
                                else
                                {
                                    resp.RESPONSE_CODE = Response::RESPONSE_TYPE::NOT_FOUND;
                                    resp.SendString("File Not Found");
								}

                            });

                    }));
                x += 155;
                if (x > 600)
                {
					x = 5;
					y += 155;
                    contents_window.SetScrollMax(y + 155);
				}
            };
            DriveRuntime::GetCurrDirFiles(ID, &add_window , [](const char* name, unsigned long long length, char isDir, void* data) {

                auto add_window = reinterpret_cast<std::function<void(std::string)>*>(data);
				(*add_window)(name);
               
            });
        }
    };

    StandardWindow::MainLoop();
    return 0;
}