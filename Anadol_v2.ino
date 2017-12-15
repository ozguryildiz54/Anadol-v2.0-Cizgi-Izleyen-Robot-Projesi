
//Kütüphane Tanımlamaları
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  
//Referans Nesnelerinin Tanımlamaları
  MDNSResponder mdns;
  ESP8266WebServer server(80);
  
//Değişken Tanımlamaları
  const char* ssid = "SUPERONLINE-WiFi_7385"; //Kullanılan kablosuz ağın ismi
  const char* password = "KKFXN9TEPR4W";  //Kullanılan kablosuz ağın şifresi
  String webPage = ""; //Web sayfasının html kodları bu değişkende tutulacaktır.
  
  //Motor sürücü pinleri tanımlandı
  const int sag_i = D2; //Sağ tekerin ileri hareketi
  const int sag_g = D3; //Sağ tekerin geri hareketi
  const int sol_i = D4; //Sol tekerin ileri hareketi
  const int sol_g = D5; //Sol tekerin geri hareketi

void setup(void)
{
  // motor sürücü pinleri çıkış olarak atandı
    pinMode(sag_i, OUTPUT); 
    pinMode(sag_g, OUTPUT);
    pinMode(sol_i, OUTPUT);
    pinMode(sol_g, OUTPUT);
    
  //Web sayfamızın html,css ve javascript kodları
    webPage += "<!DOCTYPE HTML>";
    webPage += "<html>";
    webPage += "<head>";
    webPage += "<meta charset=utf-8 />";
    webPage += "<title>Anadol V. 2.0</title>";
    webPage += "<style type=\"text/css\">#cerceve{font-size: 16px;color: #000;border-radius: 0px 39px 0px 39px;background-color: #F8F8F8;padding-top: 20px;";
    webPage += "padding-bottom: 20px;padding-right: 20px;line-height: 30px;text-align: left;box-shadow: 8px 8px 8px rgba(0, 0, 0, 0.5);}</style>";
    webPage += "<link rel=\"stylesheet\" type=\"text/css\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
    webPage += "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">";
    webPage += "<script type=\"text/javascript\" src=\"https://code.jquery.com/jquery-2.1.4.min.js\"></script>";
    webPage += "<script type=\"text/javascript\" src=\"https://cdn.rawgit.com/Foliotek/AjaxQ/master/ajaxq.js\"></script>";
    webPage += "<script type=\"text/javascript\" src=\"https://cdn.rawgit.com/marcoschwartz/aREST.js/master/aREST.js\"></script>";
    webPage += "<script type=\"text/javascript\" src=\"script.js\"></script>";
    webPage += "</head>";
    webPage += "<body>";
    webPage += "<div class='container'>";
    webPage += "<div align=\"center\"><br><img src=\"http://cuzem.cumhuriyet.edu.tr/wp-content/uploads/2017/02/cropped-kurumsal_logo-1.png\" width=125 height=125></div><br>";
    webPage += "<div style=\"text-align:center\"><p><h3>Cumhuriyet Üniversitesi Mühendislik Fakültesi</h3>";
    webPage += "<h3>Bilgisayar Mühendisliği Bölümü</h3>";
    webPage += "<h3>Gömülü Sistemler Dersi 2. Proje Ödevi</h3>";
    webPage += "<h3>Proje Adı : Anadol V. 2.0 </h3></p></div><br>";
    webPage += "<ul id=\"cerceve\">";
    webPage += "<div class='row'>";
    webPage += "<div class=\"col-md-5\"></div>";
    webPage += "<div class=\"col-md-2\">";
    webPage += "<br><a href=\"/ileri\"><button  id='forward' class='btn btn-primary btn-block' type=\"button\">İleri Git</button></a>";
    webPage += "</div>";
    webPage += "<div class=\"col-md-5\"></div>";
    webPage += "</div><br>";
    webPage += "<div class='row'>";
    webPage += "<div class=\"col-md-4\"></div>";
    webPage += "<div class=\"col-md-2\">";
    webPage += "<a href=\"/sol\"><button id='left' class='btn btn-primary btn-block' type=\"button\">Sola Dön</button></a><br>";
    webPage += "</div>";
    webPage += "<div class=\"col-md-2\">";
    webPage += "<a href=\"/sag\"><button id='right' class='btn btn-primary btn-block' type=\"button\">Sağa Dön</button></a><br>";
    webPage += "</div>";
    webPage += "<div class=\"col-md-4\"></div>";
    webPage += "</div>";
    webPage += "<div class='row'>";
    webPage += "<div class=\"col-md-5\"></div>";
    webPage += "<div class=\"col-md-2\">";
    webPage += "<a href=\"/geri\"><button id='backward' class='btn btn-primary btn-block' type=\"button\">Geri Git</button></a><br>";
    webPage += "<a href=\"/durdur\"><button id='backward' class='btn btn-primary btn-block' type=\"button\">Durdur</button></a><br>";
    webPage += "</div>";
    webPage += "<div class=\"col-md-5\"></div>";
    webPage += "</div>";
    webPage += "</body>";
    webPage += "</html>";

  //Web Servisin Kurulum Ayarları ve Çalıştırılması
    delay(1000);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Su Aga Baglanildi : ");
    Serial.println(ssid);
    Serial.print("IP adresi : ");
    Serial.println(WiFi.localIP());
    if (mdns.begin("esp8266", WiFi.localIP())) {
      Serial.println("MDNS Responder Baslatildi");
    }

   //Web Sayfasında ki butonların görevleri

    //Web sayfanın anasayfası hiçbir butona tıklanmadan yüklenir.
    server.on("/", []()
    {
      server.send(200, "text/html", webPage);
    });

    //İleri Git butonuna basıldığında aracın ileri gitmesi sağlanır.
    server.on("/ileri", []()
    {
      server.send(200, "text/html", webPage);
      digitalWrite(sag_g, LOW);
      digitalWrite(sag_i, HIGH);
      digitalWrite(sol_i, LOW);
      digitalWrite(sol_g, HIGH);   
    });

    //Geri Git butonuna basıldığında aracın geri gitmesi sağlanır.
    server.on("/geri", []()
    {
      server.send(200, "text/html", webPage);
      digitalWrite(sag_g, HIGH);
      digitalWrite(sag_i, LOW);
      digitalWrite(sol_i, HIGH);
      digitalWrite(sol_g, LOW);
    });

    //Sağa Git butonuna basıldığında aracın sağa gitmesi sağlanır.
    server.on("/sag", []()
    {
      server.send(200, "text/html", webPage);
      digitalWrite(sol_g, HIGH);
      digitalWrite(sol_i, LOW);
      digitalWrite(sag_i, LOW);
      digitalWrite(sag_g, LOW);
    });

    //Sola Git butonuna basıldığında aracın sola gitmesi sağlanır.
    server.on("/sol", []()
    {
      server.send(200, "text/html", webPage);
      digitalWrite(sol_g, LOW);
      digitalWrite(sol_i, LOW);
      digitalWrite(sag_i, HIGH);
      digitalWrite(sag_g, LOW); 
    });

    //Durdur butonuna basıldığında aracın hareketi durdurulur.
    server.on("/durdur", []()
    {
      server.send(200, "text/html", webPage);
      digitalWrite(sol_i, LOW);
      digitalWrite(sol_g, LOW);
      digitalWrite(sag_i, LOW);
      digitalWrite(sag_g, LOW); 
    });

    //Sunucu üzerine tıkladığımız butonların görevleri bittiğinde tekrar başlatılır
    server.begin();
    Serial.println("HTTP sunucusu çalıştırıldı.");
}
 
void loop(void)
{
  //Sunucu içinde gerçekleşen işlemlerin tekrar gerçekleştirilebilmesi sağlanır.
    server.handleClient();
} 
