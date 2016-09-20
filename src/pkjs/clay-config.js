//https://apps.getpebble.com/en_US/application/55b91f5cd01005e8c600007d?section=watchfaces
//https://apps.getpebble.com/en_US/application/5666e7897201eebdcc00002c?section=watchfaces

module.exports = [
	{
		"type": "heading",
		"id": "main-heading",
		"defaultValue": "CUT OUT",
		"size": 1
	},
	{
    "type": "section",
    "items": [
      { 
        "type": "heading", 
        "defaultValue": "BACKGROUND" 
      }, 
			{
				"type": "select",
				"messageKey": "IMAGE",
				"dataType": "NUMBER",
				"label": "Image",
				"defaultValue": "1", 
				"capabilities": ["COLOR"],
				"options": [
					{ 
						"label": "Ocean", 
						"value": 1, 
					},
					{ 
						"label": "Beach", 
						"value": 2, 
					},
					{ 
						"label": "Grass", 
						"value": 3, 
					},
					{ 
						"label": "Flowers", 
						"value": 4, 
					},
					{ 
						"label": "Solid (Just Color 1)", 
						"value": 30, 
					},
					{ 
						"label": "Stripes Horizontal", 
						"value": 20, 
					},
					{ 
						"label": "Stripes Vertical", 
						"value": 21, 
					},
					{ 
						"label": "Polka Dot", 
						"value": 25, 
					},
					{ 
						"label": "Checkered", 
						"value": 26, 
					},
					{ 
						"label": "Custom Image", 
						"value": 10, 
					},
				], 
			}, 
			{
				"type": "select",
				"messageKey": "IMAGE",
				"dataType": "NUMBER",
				"label": "Image",
				"defaultValue": "1", 
				"capabilities": ["BW"],
				"options": [
					{ 
						"label": "Ocean", 
						"value": 1, 
					},
					{ 
						"label": "Brick", 
						"value": 2, 
					},
					{ 
						"label": "Diagonal", 
						"value": 3, 
					},
					{ 
						"label": "Solid (Just Color 1)", 
						"value": 30, 
					},
					{ 
						"label": "Stripes Horizontal", 
						"value": 20, 
					},
					{ 
						"label": "Stripes Vertical", 
						"value": 21, 
					},
					{ 
						"label": "Polka Dot", 
						"value": 25, 
					},
					{ 
						"label": "Checkered", 
						"value": 26, 
					},
					{ 
						"label": "Custom Image", 
						"value": 10, 
					},
				], 
			}, 

			{
				"type": "input",
				"messageKey": "URL",
				"defaultValue": "",
				"label": "URL for Custom Image",
				"attributes": {
					"placeholder": "http://",
					"type": "url"
				},
				"description": "Visit <a target='_blank' href='http://matthewkingston.com/pebble/cutout/'>http://matthewkingston.com/pebble/cutout/</a> for instructions & troubleshooting",
			},
			{
				"type": "color",
				"messageKey": "COLOR_1",
				"defaultValue": "FFFF00",
				"label": "Color 1",
				"sunlight": false,
				"allowGray": false,
			},
			{
				"type": "color",
				"messageKey": "COLOR_2",
				"defaultValue": "AAAAAA",
				"label": "Color 2",
				"sunlight": false,
				"allowGray": true,
				"description": "Above Colors are for Stripe, Polka Dot, and Checkered Backgrounds",
			}
		]
	},
	{
    "type": "section",
    "items": [
      { 
        "type": "heading", 
        "defaultValue": "TIME & DATE" 
      }, 
			{
				"type": "color",
				"messageKey": "BACKGROUND",
				"defaultValue": "000000",
				"label": "Background Color",
				"sunlight": false,
				"allowGray": true,
			},
			{
				"type": "toggle",
				"messageKey": "TIME_24",
				"label": "24 Hour Time",
				"defaultValue": false
			},
			{
      	"type": "toggle",
  	    "messageKey": "FORMAT_EXTRA",
				"label": "Show Steps/Battery",
				"defaultValue": false,
				"capabilities": ["NOT_PLATFORM_APLITE"],
			},
			{
      	"type": "toggle",
  	    "messageKey": "FORMAT_TIME",
				"label": "Date on Left",
				"defaultValue": false,
			},
		{
				"type": "select",
				"messageKey": "LOCALE",
				"defaultValue": "0",
				"label": "Locale for Weekday",
				"options": [
					{ 
						"label": "English", 
						"value": "EN" 
					},
					{ 
						"label": "French",
						"value": "FR" 
					},
					{ 
						"label": "German",
						"value": "DE" 
					},
					{ 
						"label": "Spanish",
						"value": "ES" 
					},
					{ 
						"label": "Italian",
						"value": "IT" 
					},
					{ 
						"label": "Dutch",
						"value": "NL" 
					}
				]
			}, 
			{
				"type": "select",
				"messageKey": "FORMAT_DATE",
				"defaultValue": 0,
				"label": "Date Format",
				"options": [
					{ 
						"label": "Month/Day", 
						"value": 0 
					},
					{ 
						"label": "Day/Month", 
						"value": 1 
					},
				]
			}
		]
	},
	{
    "type": "submit",
    "defaultValue": "Save"
  }
];