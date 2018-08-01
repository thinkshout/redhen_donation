/**
 * @file
 * RedHen Commerce iATS direct post JavaScript functionality.
 */

(function ($) {

    Drupal.behaviors.redhen_donation_commerce_iats_dp = {
        attach: function(context, settings) {

            console.log('Attached JS: redhen_donation_commerce_iats_dp');

            // Attach click handler to populate direct post form fields on submit.
            $('#redhen-donation-form #edit-submit', context).unbind('click').bind('click', function() {

                console.log('Click handler for redhen_donation_commerce_iats_dp');

                // Populate direct post form fields.

                // IATS_DPM_ProcessID
                // IATS_DPM_Amount
                // IATS_DPM_Invoice
                // IATS_DPM_Comment
                // IATS_DPM_FirstName
                // IATS_DPM_LastName
                // IATS_DPM_Address
                // IATS_DPM_City
                // IATS_DPM_Province
                // IATS_DPM_Country
                // IATS_DPM_ZipCode

                // Method of payment.
                // IATS_DPM_MOP

                // Card number.
                // IATS_DPM_AccountNumber

                // Card expiration date.
                // IATS_DPM_ExpiryDate

                // Card security code.
                // IATS_DPM_CVV2

                // TODO: Parse card expiration date.
                //var expiry_month = $("select[name=internal_iats_dpm_exp_month]").val();
                //var expiry_year = $("select[name=internal_iats_dpm_exp_year]").val();

                //var expiry_string = expiry_month + '/' + expiry_year[2] + expiry_year[3];

                //$("input[name=IATS_DPM_ExpiryDate]").val(expiry_string);

                // TODO: Disable submit button - prevent duplicate submissions?
                return false;
            });
        }
    }

})(jQuery);
