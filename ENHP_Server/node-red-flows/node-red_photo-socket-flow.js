[
    {
        "id": "5a8bd6681a5fe18d",
        "type": "tab",
        "label": "ENP_windows",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "5472d4c70990db0f",
        "type": "group",
        "z": "5a8bd6681a5fe18d",
        "name": "Photo to socket",
        "style": {
            "label": true,
            "color": "#92d04f"
        },
        "nodes": [
            "292c74820fb8a661",
            "2cc695c1bc0ee40f",
            "2290bd7d878247d0",
            "71f05cc8faaeb81c",
            "7e707cbfc17bbf34",
            "b4711271f71588de",
            "ba58f544fed9070a",
            "f7280dccb8d11e2f",
            "18bfac7dc90f036b",
            "aa551f4a35ebbcec",
            "a3bfdfe3bce06a93",
            "585ef81c3112c722",
            "4917b353954c7b3e",
            "79dc502cd2d6327d",
            "c24509e95557cc95",
            "87d7d051e2b52821",
            "627433a7fc100cfb",
            "416039aef4634ac8",
            "1e41ee78a0b15f46",
            "02a028dabbf8251b",
            "e5607171cc7a4c14",
            "346e149c7b175cfc"
        ],
        "x": 214,
        "y": 59,
        "w": 712,
        "h": 882
    },
    {
        "id": "292c74820fb8a661",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_cauliflower",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 330,
        "y": 380,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "2cc695c1bc0ee40f",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_kale",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 300,
        "y": 540,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "2290bd7d878247d0",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_ginger",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 500,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "71f05cc8faaeb81c",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_onion",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 660,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "7e707cbfc17bbf34",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_cilantro",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 460,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "b4711271f71588de",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_cabbage",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 320,
        "y": 300,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "ba58f544fed9070a",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_broccoli",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 320,
        "y": 260,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "f7280dccb8d11e2f",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_tomato",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 900,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "18bfac7dc90f036b",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_carrot",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 340,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "aa551f4a35ebbcec",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_mushroom",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 330,
        "y": 620,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "a3bfdfe3bce06a93",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_potato",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 780,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "585ef81c3112c722",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_beet",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 220,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "4917b353954c7b3e",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_pepper",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 740,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "2ad7c7a020d1b982",
        "type": "socketio-client",
        "z": "5a8bd6681a5fe18d",
        "server": "a0ca9d3266b2690d",
        "name": "hardwaremovement_Socket",
        "x": 1100,
        "y": 320,
        "wires": [
            []
        ]
    },
    {
        "id": "79dc502cd2d6327d",
        "type": "function",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "func": "msg.eventName=\"photoresistor\";\nmsg.payload={\"board\":msg.topic,\"photo\":msg.payload};\n\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 840,
        "y": 480,
        "wires": [
            [
                "2ad7c7a020d1b982",
                "5940a5780c740a46",
                "92c2a9b4f106280a"
            ]
        ]
    },
    {
        "id": "5940a5780c740a46",
        "type": "debug",
        "z": "5a8bd6681a5fe18d",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "payload",
        "targetType": "msg",
        "statusVal": "",
        "statusType": "auto",
        "x": 1110,
        "y": 420,
        "wires": []
    },
    {
        "id": "c24509e95557cc95",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_spinach",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 320,
        "y": 860,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "87d7d051e2b52821",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_radish",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 820,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "627433a7fc100cfb",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_basil",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 180,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "416039aef4634ac8",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_parsley",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 700,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "1e41ee78a0b15f46",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_mint",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 580,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "02a028dabbf8251b",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_celery",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 310,
        "y": 420,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "e5607171cc7a4c14",
        "type": "mqtt in",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "",
        "topic": "photo_avocado",
        "qos": "2",
        "datatype": "auto",
        "broker": "d2f0ce063fa3bd21",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 320,
        "y": 140,
        "wires": [
            [
                "79dc502cd2d6327d"
            ]
        ]
    },
    {
        "id": "92c2a9b4f106280a",
        "type": "socketio-client",
        "z": "5a8bd6681a5fe18d",
        "server": "536d1c293494b81b",
        "name": "glitch_socket",
        "x": 1070,
        "y": 500,
        "wires": [
            []
        ]
    },
    {
        "id": "346e149c7b175cfc",
        "type": "comment",
        "z": "5a8bd6681a5fe18d",
        "g": "5472d4c70990db0f",
        "name": "Flow description",
        "info": "This flow listens to all the boards though MQTT. It converts the messages form topic & payload to a json {\"board\":msg.topic,\"photo\":msg.payload} to later send through sockets__",
        "x": 320,
        "y": 100,
        "wires": []
    },
    {
        "id": "d2f0ce063fa3bd21",
        "type": "mqtt-broker",
        "name": "ENP",
        "broker": "hardwaremovement.com",
        "port": "1883",
        "clientid": "",
        "autoConnect": true,
        "usetls": false,
        "protocolVersion": "4",
        "keepalive": "60",
        "cleansession": true,
        "birthTopic": "",
        "birthQos": "0",
        "birthPayload": "",
        "birthMsg": {},
        "closeTopic": "",
        "closeQos": "0",
        "closePayload": "",
        "closeMsg": {},
        "willTopic": "",
        "willQos": "0",
        "willPayload": "",
        "willMsg": {},
        "sessionExpiry": ""
    },
    {
        "id": "a0ca9d3266b2690d",
        "type": "socketio-config",
        "uri": "http://hardwaremovement.com:8080",
        "options": "{ \"forceNew\": true }"
    },
    {
        "id": "536d1c293494b81b",
        "type": "socketio-config",
        "uri": "http://networked-performance.glitch.me",
        "options": "{ \"forceNew\": true }"
    }
]
