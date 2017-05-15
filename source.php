<?php
require_once '/data/docroot/dev.leanbox.in' . '/sites/all/modules/custom/reverse_geocode/directions-api-clients-route-optimization-master/php/SwaggerClient-php/autoload.php';

$array['vehicles'][] = array('vehicle_id' => 'a1', 'start_address' => array('location_id' => 'a1', 'lon' => 12, 'lat' => 78));


Swagger\Client\Configuration::getDefaultConfiguration()->setApiKey('key', '6c0a903a-4698-42cd-aac0-52d6f5c7ee0a');

// Uncomment below to setup prefix (e.g. Bearer) for API key, if needed
// Swagger\Client\Configuration::getDefaultConfiguration()->setApiKeyPrefix('key', 'Bearer');

$api_instance = new Swagger\Client\Api\VrpApi();
$key = "6c0a903a-4698-42cd-aac0-52d6f5c7ee0a"; // string | your API key

$body = new \Swagger\Client\Model\Request($array); // \Swagger\Client\Model\Request | Request object that contains the problem to be solved


try {
    $result = $api_instance->postVrp($key, $body);
    print_r($result);
} catch (Exception $e) {
    echo 'Exception when calling VrpApi->postVrp: ', $e->getMessage(), PHP_EOL;
}

?>
