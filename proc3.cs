using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace PPR_zad_indywidualne
{
    class proc3
    {
		static void SoapClient(string msg) {
			var binding = new System.ServiceModel.BasicHttpBinding();
            binding.Name = "simpleService_webservices";
            binding.Namespace = "pl.pc.wat.ppr.wsdl";
            var endpointAddress = new System.ServiceModel.EndpointAddress(new Uri("http://127.0.0.1:1234"));
            var ssr = new ServiceReference.sendMessageClient(binding,endpointAddress);
            var req = new ServiceReference.sendMessageRequest("msg.ToString()");
            Console.WriteLine("Przeslano");
			Console.WriteLine(msg);
            var result = ssr.sendMessageAsync(req);
            result.Wait();
		}

        static void Main(string[] args)
        {
            byte[] bytes = new Byte[1024];
			string msg = null;
			IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, 2222);
			Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp );
			Console.WriteLine("*** PROCES 3 ***");
			listener.Bind(localEndPoint);
			listener.Listen(10); 
				while (true) {
					try{
						Console.WriteLine("Waiting...");
						Socket handler = listener.Accept();
						handler.Receive(bytes);
						//msg = Encoding.Unicode.GetString(bytes);
                        msg = System.Text.Encoding.Default.GetString(bytes);
						Console.WriteLine($"Received: {msg}, sending it back.");
						SoapClient(msg);
						handler.Shutdown(SocketShutdown.Both);
						handler.Close();
					} catch (Exception e) {
						Console.WriteLine(e.ToString());
					}
				}
            }
        }
}