
ABOUT
-----
Module provides request-specific token authorisation method for the Services module.


INSTALLATION
------------
1. Enable the module.
2. Edit a Services endpoint and enable "Request-specific token authorisation" in the "Authentication" section.
3. On the "Authentication" tab, provide the "Token key" value, and optionally change the "Token header name"
   default value ("X-Services-Token") if required.


USAGE
-----
When calling a Services endpoint with request-specific token authorisation enabled,
you should add the "X-Services-Token" header to your request, which value should be calculated as:

$x_services_token = sha1(<token_key_value> . <request_body>);
