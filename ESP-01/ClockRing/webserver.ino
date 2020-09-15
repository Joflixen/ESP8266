
void webClient(WiFiClient wc) {
  String request = wc.readStringUntil('\r');
  wc.flush();

  // Return response
  wc.println("HTTP/1.1 200 OK");
  wc.println("Content-Type: text/html");
  wc.println("");
  wc.println("<html>");
  wc.print("Time is: "); wc.print(clock_hour); wc.print(":"); wc.print(clock_minute); wc.print(":"); wc.print(clock_second);
  wc.println("</html>");
}
