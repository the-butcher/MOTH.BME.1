package com.igorion.mqtt;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttSecurityException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import com.igorion.bme.BmeRawData;

/**
 * simple implementation for receiving MQTT messages from a bme688_client_01 device
 *
 * @author h.fleischer
 * @since 18.08.2023
 *
 */
public class MqttReaderPahoBme {

    public static final String MQTT_HOST = "tcp://192.168.0.115:1883";
    public static final String MQTT_CLID = "bme_helper";

    public static final String MQTT_TOPIC_HEATER = "bmeheater";
    public static final String MQTT_TOPIC_VALUES = "bmevalues";

    protected static BmeRawData BME_RAW_DATA;

    @SuppressWarnings("resource")
    public static void main(String[] args) throws Exception {

        MqttClient client = new MqttClient(MQTT_HOST, MQTT_CLID, new MemoryPersistence());
        MqttConnectOptions options = new MqttConnectOptions();
        options.setHttpsHostnameVerificationEnabled(false);

        client.setCallback(new MqttCallback() {

            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("connectionLost: " + cause.getMessage() + ", trying to reconnect");
                try {
                    connect(client, options);
                } catch (Exception ex) {
                    throw new RuntimeException("failed to reonnect", ex);
                }
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) {
                try {
                    if (topic.equals(MQTT_TOPIC_HEATER)) {
                        System.out.println(String.format("%s :: reveived", MQTT_TOPIC_HEATER));
                        BME_RAW_DATA = new BmeRawData(message.getPayload());
                    } else if (topic.equals(MQTT_TOPIC_VALUES)) {
                        System.out.println(String.format("%s :: reveived", MQTT_TOPIC_VALUES));
                        if (BME_RAW_DATA != null) {
                            BME_RAW_DATA.acceptData(message.getPayload());
                        }
                    }
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                System.out.println("deliveryComplete" + token.isComplete());
            }

        });

        connect(client, options);

    }

    public static void connect(MqttClient client, MqttConnectOptions options) throws MqttSecurityException, MqttException {
        client.connect(options);
        client.subscribe(MQTT_TOPIC_HEATER);
        client.subscribe(MQTT_TOPIC_VALUES);
        System.out.println("reader :: connected");
    }

}
