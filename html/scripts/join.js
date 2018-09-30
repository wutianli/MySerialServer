$(document).ready(
	function()
	{
		$("#landing").submit
				(			
					function(  )
					{		
						var text_id=new Array("audio_alert");
						jQuery.ajax({
							url: "./cgi-bin/landing.cgi",
							data: $('#landing').serialize(  ),
							type: "POST",
							dateType: "json",
							beforeSend: function( )
							{
								if((document.getElementById("username").value=="")||(document.getElementById("password").value==""))
								{
									alert("账号或者密码为空，请重新输入！");
									return false;
								}
							},
							error: function(request)  			
							{
								alert("登录失败！");
							},
							
							success: function(data) 
							{
			
								var all_data = data.split(':');
								if(all_data[0]=="1")
								{
										//window.location.href= "version.html" ; 
　									　	url = "version.html?s=1";
　　									location.href=url;
								}
								else
								{
									alert("账号或者密码错误");
								}
							}
						});		
						return false;				
					}		
				);
		$("#mod_landing").submit
				(			
					function(  )
					{		
						var text_id=new Array("audio_alert");
						jQuery.ajax({
							url: "./cgi-bin/mod_landing.cgi",
							data: $('#mod_landing').serialize(  ),
							type: "POST",
							dateType: "json",
							beforeSend: function( )
							{
								if((document.getElementById("zhanghao_").value=="")||(document.getElementById("mima_").value==""))
								{
									alert("账号或者密码为空，请重新输入！");
									return false;
								}
								
								if((document.getElementById("mima_2").value)!=(document.getElementById("mima_").value))
								{
									alert("两次输入不一致，请重新输入！");
									return false;
								}
								
							},
							error: function(request)  			
							{
								alert("修改失败");
							},
							
							success: function(data) 
							{
			
								var all_data = data.split(':');
								if(all_data[0]=="1")
								{
									alert("账号密码修改成功！");
									
								}
								else
								{
									alert("账号密码修改失败");
								}
							}
						});		
						return false;				
					}		
				);
		}
);