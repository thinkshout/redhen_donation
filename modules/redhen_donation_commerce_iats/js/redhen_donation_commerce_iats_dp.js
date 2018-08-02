/**
 * @file
 * RedHen Commerce iATS direct post JavaScript functionality.
 */

(function ($) {

    Drupal.behaviors.redhen_donation_commerce_iats_dp = {
        attach: function(context, settings) {
            // Attach click handler to populate direct post form fields on submit.
            $('#redhen-donation-form #edit-submit', context).unbind('click').bind('click', function() {
                // Populate direct post form fields.

                // IATS_DPM_Amount
                // TODO: Needs to work for custom donation amounts.
                var donation = $("select[name=donation]").val();
                $("input[name=IATS_DPM_Amount]").val(donation);

                // TODO: Invoice number.
                // Invoice number isn't available before order is created.
                // Order is created after form submission. This needs work.
                // IATS_DPM_Invoice

                // TODO: Store order ID. Append, do not overwrite.
                // Get order ID by creating donation order via AJAX.
                // IATS_DPM_Comment

                // IATS_DPM_FirstName
                var first_name = $("input[name=first_name]").val();
                $("input[name=IATS_DPM_FirstName]").val(first_name);

                // IATS_DPM_LastName
                var last_name = $("input[name=last_name]").val();
                $("input[name=IATS_DPM_LastName]").val(last_name);

                // IATS_DPM_Address
                var address = $("input[name='commerce_customer_address[und][0][thoroughfare]']").val();
                $("input[name=IATS_DPM_Address]").val(address);

                // IATS_DPM_City
                var city = $("input[name='commerce_customer_address[und][0][locality]']").val();
                $("input[name=IATS_DPM_City]").val(city);

                // State.
                // IATS_DPM_Province
                var state = $("select[name='commerce_customer_address[und][0][administrative_area]']").val();
                $("input[name=IATS_DPM_Province]").val(state);

                // IATS_DPM_Country
                var country = $("select[name='commerce_customer_address[und][0][country]']").val();
                $("input[name=IATS_DPM_Country]").val(country);

                // Postal code.
                // IATS_DPM_ZipCode
                var zip = $("input[name='commerce_customer_address[und][0][postal_code]']").val();
                $("input[name=IATS_DPM_ZipCode]").val(zip);

                // Method of payment.
                // IATS_DPM_MOP
                var mop = $("select[name='payment_details[credit_card][type]']").val();
                $("input[name=IATS_DPM_MOP]").val(mop);

                // Card number.
                // IATS_DPM_AccountNumber
                var number = $("input[name='payment_details[credit_card][number]']").val();
                $("input[name=IATS_DPM_AccountNumber]").val(number);

                // Card expiration date.
                // IATS_DPM_ExpiryDate

                var expiry_month = $("select[name='payment_details[credit_card][exp_month]']").val();
                var expiry_year = $("select[name='payment_details[credit_card][exp_year]']").val();

                var expiry_string = expiry_month + '/' + expiry_year[2] + expiry_year[3];

                $("input[name=IATS_DPM_ExpiryDate]").val(expiry_string);

                // Card security code.
                // IATS_DPM_CVV2
                var cvv2 = $("input[name='payment_details[credit_card][code]']").val();
                $("input[name=IATS_DPM_CVV2]").val(cvv2);

                // TODO: Post payment information to iATS.

                // TODO: May need to remove all non direct post fields.
                // POSTing random stuff to iATS might break something.

                // Disable submit button - prevent duplicate submissions.
                return false;
            });
        }
    }

})(jQuery);
