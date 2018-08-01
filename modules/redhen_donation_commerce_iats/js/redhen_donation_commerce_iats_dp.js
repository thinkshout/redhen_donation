/**
 * @file
 * RedHen Commerce iATS direct post JavaScript functionality.
 */

(function ($) {

    Drupal.behaviors.redhen_donation_commerce_iats_dp = {
        attach: function(context, settings) {
            // Attach click handler to populate direct post form elements on submit.
            $('.checkout-continue', context).unbind('click').bind('click', function() {

                console.log('redhen_donation_commerce_iats_dp');

                //var expiry_month = $("select[name=internal_iats_dpm_exp_month]").val();
                //var expiry_year = $("select[name=internal_iats_dpm_exp_year]").val();

                //var expiry_string = expiry_month + '/' + expiry_year[2] + expiry_year[3];

                //$("input[name=IATS_DPM_ExpiryDate]").val(expiry_string);

                return true;
            });
        }
    }

})(jQuery);
