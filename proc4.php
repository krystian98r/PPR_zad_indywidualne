<?php
	function strToHex($string){
		$hex='';
		for ($i=0; $i < strlen($string); $i++){
			$hex .= dechex(ord($string[$i]));
		}
		return $hex;
	}
	#===================================================================
	function sendMessage($mess) {
		$stderr = fopen('php://stderr', 'w');

		$hex = strToHex($mess);
		// fwrite( $stderr, "Message: $hex\n" );
		fwrite( $stderr, "Message: $mess\n" );
		return $hex;
	}
	$server = new SoapServer("auto.wsdl");
	$server->addFunction("sendMessage");
	$server->handle();
	#===================================================================

?>

