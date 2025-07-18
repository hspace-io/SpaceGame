package this_is_real_flag;

import flash.Lib;

import flash.display.Shape;
import flash.display.MovieClip;
import flash.display.Sprite;

import flash.text.TextField;
import flash.text.TextFieldType;
import flash.text.TextFormat;
import flash.text.TextFormatAlign;

import flash.events.MouseEvent;
/**
 * ...
 * @author 5un9hun
 */

class Main extends MovieClip {
	public var entire_stageWidth:Int;
    public var entire_stageHeight:Int;
	public var this_is_format_zzlol:TextFormat;

    public function new() {
		super();
		Round_ONEONEONE();
    }

    function showResultFrame(user_input:TextField) {
        var flag_result = new TextField();
		
		flag_result.width = 1000;
		flag_result.height = 300;
		flag_result.x = (entire_stageWidth - flag_result.width) / 2;
		flag_result.y = (entire_stageHeight - flag_result.height) / 1.3;
		this_is_format_zzlol = new TextFormat();
		this_is_format_zzlol.align = TextFormatAlign.CENTER;
        
		var challenge = new CheckFlag();
		challenge.check(user_input.text);
		
		if (!challenge.status_code) {
			graphics.beginFill(0xFF8080);
			graphics.drawRect(0, 0, entire_stageWidth, entire_stageHeight);
			graphics.endFill();
			
			this_is_format_zzlol.size = 100; 

			flag_result.text = "Wrong ...";

			flag_result.defaultTextFormat = this_is_format_zzlol;
			flag_result.setTextFormat(this_is_format_zzlol);
			
			addChild(flag_result);
		}
		else {
			graphics.beginFill(0x80FF80);
			graphics.drawRect(0, 0, entire_stageWidth, entire_stageHeight);
			graphics.endFill();
			
			this_is_format_zzlol.size = 100; 
			
			flag_result.text = "Correct!";

			flag_result.defaultTextFormat = this_is_format_zzlol;
			flag_result.setTextFormat(this_is_format_zzlol);
			
			addChild(flag_result);
			
			var sakdjlfhjdksla = new TextField();
			sakdjlfhjdksla.width = 1000;
			sakdjlfhjdksla.height = 150;
			sakdjlfhjdksla.text = "Flag is\nhspace{" + user_input.text + "}";
			
			sakdjlfhjdksla.x = (entire_stageWidth - sakdjlfhjdksla.width) / 2;
			sakdjlfhjdksla.y = (entire_stageHeight - sakdjlfhjdksla.height) / 1.2;
			
			var oserukhasdkja = new TextFormat();
			oserukhasdkja.size = 30;
			oserukhasdkja.align = TextFormatAlign.CENTER;
			sakdjlfhjdksla.defaultTextFormat = oserukhasdkja;
			sakdjlfhjdksla.setTextFormat(oserukhasdkja);
			addChild(sakdjlfhjdksla);
		}
    }
	
	public function Round_ONEONEONE() {
		
		entire_stageWidth = Lib.current.stage.stageWidth;
        entire_stageHeight = Lib.current.stage.stageHeight;
		
		graphics.beginFill(0xFFFF00);
		graphics.drawRect(0, 0, entire_stageWidth, entire_stageHeight);
		graphics.endFill();
		
		var desc = new TextField();
		desc.text = "Submit your flag.";
		desc.width = 1000;
		desc.height = 80;
		desc.x = (entire_stageWidth - desc.width) / 2;
        desc.y = (entire_stageHeight - desc.height) / 3;
		
		this_is_format_zzlol = new TextFormat();
		this_is_format_zzlol.size = 50;
		this_is_format_zzlol.align = TextFormatAlign.CENTER;
		
		desc.defaultTextFormat = this_is_format_zzlol;
		desc.setTextFormat(this_is_format_zzlol);
		
        addChild(desc);
		
        var user_input = new TextField();
		
		// EditText
        user_input.type = flash.text.TextFieldType.INPUT;
        user_input.border = true;
        user_input.width = 500;
        user_input.height = 30;
        user_input.x = (entire_stageWidth - user_input.width) / 2;
        user_input.y = (entire_stageHeight - user_input.height) / 1.9;
		
		this_is_format_zzlol = new TextFormat();
		this_is_format_zzlol.size = 24; 
		user_input.defaultTextFormat = this_is_format_zzlol;
		user_input.setTextFormat(this_is_format_zzlol);
		
        addChild(user_input);

		// Button
        var check_button = new Sprite();
        check_button.graphics.beginFill(0x3399FF);
        check_button.graphics.drawRoundRect(0, 0, 120, 40, 10);
        check_button.graphics.endFill();
        check_button.x = (entire_stageWidth - check_button.width) / 2;
        check_button.y = (entire_stageHeight - check_button.height) / 1.5;
        check_button.buttonMode = true;
        addChild(check_button);

		// Button Label
        var btn_label_enter = new TextField();
        btn_label_enter.text = "Enter";
        btn_label_enter.width = check_button.width;
        btn_label_enter.height = check_button.height;
		btn_label_enter.x = (check_button.width - btn_label_enter.textWidth) / 2 - 4;
        btn_label_enter.y = (check_button.height - btn_label_enter.textHeight) / 2 - 4;
        btn_label_enter.selectable = false;

		this_is_format_zzlol.size = 16; 
		btn_label_enter.defaultTextFormat = this_is_format_zzlol;
		btn_label_enter.setTextFormat(this_is_format_zzlol);
		
        check_button.addChild(btn_label_enter);

        check_button.addEventListener(MouseEvent.CLICK, function(e:MouseEvent) {
			removeChild(desc);
			removeChild(user_input);
			removeChild(check_button);
			showResultFrame(user_input);
		});
    }

    static public function main() {
        flash.Lib.current.addChild(new Main());
    }
}


