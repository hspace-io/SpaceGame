package this_is_real_flag;

/**
 * ...
 * @author 5un9hun
 */
class CheckFlag {

	public var status_code:Bool;
	var enc_result_value = [0, 0, 0, 0];
	var BASE32_ALPHABET = "ervngpacwidfbuzhklmhso_954207381";
	var this_is_key = "spaceWar";
	var zzfinal_result = [131, 210, 106, 164, 162, 243, 124, 13, 168, 96, 219, 14, 198, 186, 129, 129, 143, 253, 186, 62];
	
	public function new() {
		this.status_code = true;
	}
	
	// custom base32 decode
	public function custom_this_is_base32_decode(user_input:String):haxe.io.Bytes {
		var nwoinmnortjto = StringTools.trim(user_input).split("=").join("");
		var ioqwunvkmcn = 0;
		var poierukdhjkt = 0;
		var nvmcruiduriew = new haxe.io.BytesBuffer();
		
		for (i in 0...nwoinmnortjto.length) {
			var ch = nwoinmnortjto.charAt(i);
			if (BASE32_ALPHABET.indexOf(ch) == -1) return null;
			poierukdhjkt = (poierukdhjkt << 5) | BASE32_ALPHABET.indexOf(ch);
			ioqwunvkmcn += 5;
			if (ioqwunvkmcn >= 8) {
				nvmcruiduriew.addByte((poierukdhjkt >> (ioqwunvkmcn - 8)) & 0xFF);
				ioqwunvkmcn -= 8;
			}
		}
		return nvmcruiduriew.getBytes();
	}
	
	// bit swap
	public function bit_swap_zzlol_real_bit_swap(data_zz_data:haxe.io.Bytes):haxe.io.Bytes {
		var oquklnvmkenr = new haxe.io.BytesBuffer();
		for (i in 0...data_zz_data.length) {
			var bvmeirjhtjk = 0;
			var b = data_zz_data.get(i);
			if (b < 0 || b > 255) throw "Invalid byte value";
			for (j in 0...8) {
				bvmeirjhtjk = (bvmeirjhtjk << 1) | ((b >> j) & 1);
			}
			oquklnvmkenr.addByte(bvmeirjhtjk);
		}
		return oquklnvmkenr.getBytes();
	}
	
	// xor
	public function is_this_real_xor_zz_yes_this_is_xor(user_input:haxe.io.Bytes) {
		var biepowjkerj = new haxe.io.BytesBuffer();
		for (i in 0...user_input.length) {
			biepowjkerj.addByte(user_input.get(i) ^ this_is_key.charCodeAt(i % this_is_key.length));
		}
		return biepowjkerj.getBytes();
	}
	
	public function check(user_input:String) {
		
		if (user_input == null || user_input.length % 8 != 0) {
			this.status_code = false;
			return;
		}
		
		var qoierupiohfdfjg = custom_this_is_base32_decode(user_input);
		
		if (qoierupiohfdfjg == null || qoierupiohfdfjg.length != 0x14) {
			this.status_code = false;
			return;
		}
		
		var sakjdfhaskjd = bit_swap_zzlol_real_bit_swap(qoierupiohfdfjg);
		var dfskdhaksrre = is_this_real_xor_zz_yes_this_is_xor(sakjdfhaskjd);
		for (i in 0...zzfinal_result.length) {
			if (dfskdhaksrre.get(i) != zzfinal_result[i]) {
				this.status_code = false;
				return;
			}
		}
		this.status_code = true;
	}
	
	public function dummy1jkhdfkljasfd(sajhdlf:Int) {
		var askjldrkjlpqoi = 0x3000;
		dummy2jkhdfkljasfd(askjldrkjlpqoi);
	}
	
	public function dummy2jkhdfkljasfd(sajhdlf:Int) {
		var askjldrkjlpqoi = sajhdlf * 21912 % sajhdlf;
		dummy4jkhdfkljasfd(askjldrkjlpqoi);
	}
	
	public function dummy3jkhdfkljasfd(sajhdlf:Int) {
		var askjldrkjlpqoi = sajhdlf * 31275 % sajhdlf;
		dummy5jkhdfkljasfd(askjldrkjlpqoi);
		
	}
	
	public function dummy4jkhdfkljasfd(sajhdlf:Int) {
		var askjldrkjlpqoi = sajhdlf * 41252 % sajhdlf;
		dummy3jkhdfkljasfd(askjldrkjlpqoi);
	}
	
	public function dummy5jkhdfkljasfd(sajhdlf:Int) {
		var askjldrkjlpqoi = sajhdlf * 54521 % sajhdlf;
		dummy1jkhdfkljasfd(askjldrkjlpqoi);
	}
}