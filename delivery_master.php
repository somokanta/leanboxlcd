<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

$remote_file = "abc_testing.csv";
 
/* FTP Account (Remote Server) */
$ftp_host = "103.233.76.251"; /* host */
$ftp_user_name = "sftpuser03"; /* username */
$ftp_user_pass = "Techub123#"; /* password */
 
 
/* File and path to send to remote FTP server */
$local_file = "/data/docroot/hul.leanbox.in/sites/default/files/TP2IMPACT_DELIVERY/TP2IMPACT_DELIVERY_20170725111918_26072017.csv";
 
/* Connect using basic FTP */
$connect_it = ftp_connect( $ftp_host ,22);
 var_dump($connect_it);
/* Login to FTP */
$login_result = ftp_login( $connect_it, $ftp_user_name, $ftp_user_pass );
 var_dump($login_result);
/* Send $local_file to FTP */
if ( ftp_put( $connect_it, $remote_file, $local_file, FTP_BINARY ) ) {
    echo "WOOT! Successfully transfer $local_file\n";
}
else {
    echo "Doh! There was a problem\n";
}
 
/* Close the connection */
ftp_close( $connect_it );