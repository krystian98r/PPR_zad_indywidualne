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
            var endpointAddress = new System.ServiceModel.EndpointAddress(new Uri("http://127.0.0.1:3333"));
            var ssr = new ServiceReference.sendMessageClient(binding,endpointAddress);
            var req = new ServiceReference.sendMessageRequest(msg);
            Console.WriteLine("Sent forward");
            var result = ssr.sendMessageAsync(req);
            result.Wait();
		}

        static void Main(string[] args)
        {	
			Console.WriteLine("*** PROCES 3 ***");
			byte[] bytes = new Byte[1024];
			string msg;
			IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, 2222);
			Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp );
			try {
				listener.Bind(localEndPoint);
				listener.Listen(10); 
				while (true) {
					try{
						Socket handler = listener.Accept();
						handler.Receive(bytes);
						msg = System.Text.Encoding.Default.GetString(bytes);
						// msg = Encoding.Unicode.GetString(bytes);
						// Console.WriteLine(msg);
						byte[] ba = Encoding.Default.GetBytes(msg);
						string hexString = BitConverter.ToString(bytes);
						hexString = hexString.Replace("-", "");
						handler.Close();
						SoapClient(hexString.TrimEnd('0'));
					} catch (Exception e) {
						Console.WriteLine(e.ToString());
					} 
				}
			} catch (Exception e) {
				Console.WriteLine(e.ToString());
			}
        }
        }
}