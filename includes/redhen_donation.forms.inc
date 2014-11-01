<?php
/**
 * @file
 * Form include.
 */

/**
 * Return a form for an entity's donation settings.
 *
 * @internal object $entity
 *   Donation Entity.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param array $settings
 *   Settings array.
 * @param string $entity_type
 *   Host entity type.
 * @param null $entity_id
 *   Host entity id.
 *
 * @see hook_redhen_donation_entity_settings()
 *
 * @return array
 *   Form array.
 */
function redhen_donation_entity_settings_form($form, &$form_state, $settings, $entity_type = NULL, $entity_id = NULL) {
  if ($entity_id) {
    // We'll need this info when we submit the form:
    $form_state['entity'] = array(
      'entity_id' => $entity_id,
      'entity_type' => $entity_type,
    );
  }

  // Check to see if date_popup is installed so we can provide a friendlier UI.
  $date_popup_installed = FALSE;
  if (module_exists('date_popup')) {
    $date_popup_installed = TRUE;
  }
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#description' => t('Check to enable donations.'),
    '#default_value' => isset($settings['status']) ? $settings['status'] : -1,
  );

  $form['scheduling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scheduling'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['scheduling']['open'] = array(
    '#type' => 'textfield',
    '#title' => t('Open Date'),
    '#maxlength' => 25,
    '#description' => t('When to automatically open donations. Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to disable.', array(
        '%time' => !empty($settings['open']) ? date_format(date_create($settings['open']), 'Y-m-d H:i:s O') : format_date(time(), 'custom', 'Y-m-d H:i:s O'),
        '%timezone' => !empty($settings['open']) ? date_format(date_create($settings['open']), 'O') : format_date(time(), 'custom', 'O'),
      )
    ),
    '#default_value' => !empty($settings['open']) ? $settings['open'] : '',
  );
  if ($date_popup_installed) {
    $form['scheduling']['open']['#type'] = 'date_popup';
    $form['scheduling']['open']['#format'] = 'Y-m-d H:i:s O';
    $form['scheduling']['open']['#description'] = t('When to automatically open donations. (This uses the !timezone.)', array(
      '!timezone' => l(t('site default time zone'), 'admin/config/regional/settings')));
    unset($form['scheduling']['open']['#maxlength']);
  }

  $form['scheduling']['close'] = array(
    '#type' => 'textfield',
    '#title' => t('Close Date'),
    '#maxlength' => 25,
    '#description' => t('When to automatically close donations. Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to disable.', array(
        '%time' => !empty($settings['close']) ? date_format(date_create($settings['close']), 'Y-m-d H:i:s O') : format_date(time(), 'custom', 'Y-m-d H:i:s O'),
        '%timezone' => !empty($settings['close']) ? date_format(date_create($settings['close']), 'O') : format_date(time(), 'custom', 'O'),
      )
    ),
    '#default_value' => !empty($settings['close']) ? $settings['close'] : '',
  );
  if ($date_popup_installed) {
    $form['scheduling']['close']['#type'] = 'date_popup';
    $form['scheduling']['close']['#format'] = 'Y-m-d H:i:s O';
    $form['scheduling']['close']['#description'] = t('When to automatically close donations. (This uses the !timezone.)', array(
      '!timezone' => l(t('site default time zone'), 'admin/config/regional/settings')));
    unset($form['scheduling']['close']['#maxlength']);
  }

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['settings']['confirmation_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation page'),
    '#default_value' => isset($settings['settings']['confirmation_page']) ? $settings['settings']['confirmation_page'] : '',
    '#description' => t('Set the URL of the confirmation page. If not set, the form will redirect to itself.'),
  );

  $form['settings']['confirmation_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation message'),
    '#default_value' => isset($settings['settings']['donation_settings']['confirmation_message']) ? $settings['settings']['donation_settings']['confirmation_message'] : 'Thank you for your donation!',
    '#description' => t('Set the message to appear after a successful donation. Leave blank to display no message.'),
  );

  $form['settings']['donation_entry'] = array(
    '#type' => 'select',
    '#title' => t('Donation entry'),
    '#default_value' => isset($settings['settings']['donation_entry']) ? $settings['settings']['donation_entry'] : '',
    '#options' => array(
      'select' => t('Select list'),
      'select_other' => t("Select list (with 'other' option)"),
      'radio_other' => t("Radio options (with 'other' option)"),
      'custom' => t('Text field (custom amount)'),
    ),
    '#description' => t('Select the type of options widget to use for donation amount.'),
  );

  $donation_amounts = '';
  if (isset($settings['settings']['donation_amounts'])) {
    if (is_array($settings['settings']['donation_amounts'])) {
      $donation_amounts = redhen_allowed_amounts_string($settings['settings']['donation_amounts']);
    }
    else {
      $donation_amounts = $settings['settings']['donation_amounts'];
    }
  }

  $form['settings']['donation_amounts'] = array(
    '#type' => 'textarea',
    '#title' => t('Donation amounts'),
    '#default_value' => $donation_amounts,
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[donation_entry]"]' => array(
          'value' => 'custom',
        ),
      ),
    ),
    '#description' => t('List of allowed donation amounts. Enter one value per line, in the format amount|label. The amount should be entered as an integer value.'),
  );

  $form['settings']['minimum_donation'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum donation'),
    '#default_value' => isset($settings['settings']['minimum_donation']) ? $settings['settings']['minimum_donation'] : '',
    '#description' => t('Enter the minimum allowed donation as an integer. Leave blank for no minimum.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[donation_entry]"]' => array(
          'value' => 'select',
        ),
      ),
    ),
  );

  $form['settings']['maximum_donation'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum donation'),
    '#default_value' => isset($settings['settings']['maximum_donation']) ? $settings['settings']['maximum_donation'] : '',
    '#description' => t('Enter the maximum allowed donation as an integer. Leave blank for no maximum.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[donation_entry]"]' => array(
          'value' => 'select',
        ),
      ),
    ),
  );

  $form['settings']['submit_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Submit button'),
    '#default_value' => isset($settings['settings']['submit_label']) ? $settings['settings']['submit_label'] : 'Donate',
    '#description' => t('The text on the form submission button.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[donation_entry]"]' => array(
          'value' => 'select',
        ),
      ),
    ),
  );

  // Allow other modules to add their own custom settings:
  $form['settings'] += module_invoke_all('redhen_donation_entity_settings', $settings);

  // Only show save if we're not on the field instance defaults.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
    '#access' => !empty($entity_id),
  );

  return $form;
}

/**
 * Validation handler for redhen_donation_entity_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function redhen_donation_entity_settings_form_validate($form, &$form_state) {
  // Extract allowed amounts.
  if ($form_state['values']['settings']['donation_entry'] == 'select' ||
    $form_state['values']['settings']['donation_entry'] == 'select_other' ||
    $form_state['values']['settings']['donation_entry'] == 'radio_other') {
    $form_state['values']['settings']['donation_amounts'] = redhen_extract_allowed_amounts($form_state['values']['settings']['donation_amounts']);
    if (empty($form_state['values']['settings']['donation_amounts'])) {
      form_set_error('settings][donation_amounts', t('No amount(s) set.'));
    }
    // Validate allowed amounts.
    foreach ($form_state['values']['settings']['donation_amounts'] as $amount => $label) {
      if (!is_numeric($amount)) {
        form_set_error('settings][donation_amounts', t('Donation amounts are invalid.'));
      }
    }
  }

  // Validate open date:
  if (!empty($form_state['values']['scheduling']['open']) && strtotime($form_state['values']['scheduling']['open']) === FALSE) {
    form_set_error('scheduling][open', t('Date is invalid.'));
  }

  // Validate close date:
  if (!empty($form_state['values']['scheduling']['close']) && strtotime($form_state['values']['scheduling']['close']) === FALSE) {
    form_set_error('scheduling][close', t('Date is invalid.'));
  }
}

/**
 * Submit handler for redhen_donation_entity_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function redhen_donation_entity_settings_form_submit($form, &$form_state) {
  $entity = $form_state['entity'];

  $fields = redhen_donation_convert_form_settings($form_state['values']);

  redhen_donation_update_entity_settings($entity['entity_type'], $entity['entity_id'], $fields);

  $uri = entity_uri($entity['entity_type'], entity_load_single($entity['entity_type'], $entity['entity_id']));

  $form_state['redirect'] = $uri['path'];
}

/**
 * Form callback: create or edit a donation.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param RedhenDonation $donation
 *   The donation object to edit or for a create form and empty donation
 *   object with an attached entity defined.
 *
 * @return mixed
 *   Donation form array.
 */
function redhen_donation_form($form, &$form_state, RedhenDonation $donation) {
  global $user;

  $form_state['donation'] = $donation;

  if (!isset($form_state['#donation_settings'])) {
    $form_state['donation_settings']
      = $settings
        = redhen_donation_entity_settings(
            $donation->entity_type,
            $donation->entity_id,
            TRUE
          );
  }

  $donation_type = redhen_donation_type_load($donation->bundle());
  $contact_settings = $donation_type->settings['contact_settings'];
  $commerce_settings = $donation_type->settings['commerce_settings'];

  $contact_object = redhen_contact_load_by_user($user);
  if (!$contact_object) {
    $contact_object = redhen_contact_create(array('type' => $contact_settings['contact_type']));
  }
  $contact_wrapper = entity_metadata_wrapper('redhen_contact', $contact_object);

  module_load_include('inc', 'redhen_contact', 'includes/redhen_contact.forms');

  // Get contact type form.
  $form['redhen_contact'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Information'),
    'form' => redhen_contact_contact_form(array(), $form_state, $contact_object),
  );

  // Unset the contact forms action, we will use the registration form.
  unset($form['redhen_contact']['form']['actions']);
  // Hide all fields by default.
  $form_fields = element_children($form['redhen_contact']['form']);
  foreach ($form_fields as $form_field) {
    if (strpos($form_field, 'field_') !== FALSE || $form_field == 'redhen_contact_email') {
      $form['redhen_contact']['form'][$form_field]['#access'] = FALSE;
    }
  }

  // Remove fields as defined in settings.
  $fields = field_info_instances('redhen_contact', $contact_settings['contact_type']);
  foreach ($fields as $field) {
    $field_info = field_info_field($field['field_name']);
    // Check for field settings.
    $field_setting = isset($settings['settings']['redhen_fields']['fields'][$field_info['field_name']])
      ? $contact_settings['fields'][$field_info['field_name']]
      : NULL;
    if ($field_info['type'] != 'addressfield' && $field_setting) {
      $form['redhen_contact']['form'][$field_info['field_name']]['#access'] = TRUE;
    }

    $middle_name = ($contact_settings['fields']['middle_name'] === 'middle_name');

    $form['redhen_contact']['form']['name']['middle_name']['#access'] = $middle_name;
    unset($form['redhen_contact']['form']['redirect']);
  }

  $form['redhen_contact']['form']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#default_value' => $contact_wrapper->email->value(),
  );

  field_attach_form('redhen_donation', $donation, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($commerce_settings['donation_settings']['submit_label']) ? $commerce_settings['donation_settings']['submit_label'] : t('Donate'),
  );

  $payment_methods = array();
  foreach ($commerce_settings['payment_methods'] as $payment_method) {
    $payment_methods[$payment_method['payment_method']] = commerce_payment_method_instance_load($payment_method['payment_method']);
  }
  $options = array();

  $payment_methods_new = $commerce_settings['payment_methods'];
  foreach ($payment_methods_new as $payment_method_new) {
    $options[$payment_method_new['payment_method']] = $payment_method_new['label'];
  }
  $first_payment_option = current(current($payment_methods_new));

  // Add a radio select widget to specify the payment method.
  if (count($payment_methods) > 1) {
    $form['payment_method'] = array(
      '#title' => t('Payment Options'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $first_payment_option,
      '#ajax' => array(
        'callback' => 'redhen_donation_form_refresh',
        'wrapper' => 'payment-details',
      ),
    );
  }
  else {
    $form['payment_method'] = array(
      '#type' => 'hidden',
      '#value' => $first_payment_option,
    );
  }

  // Handle currency mapping.
  $top_payment = current($commerce_settings['payment_methods']);
  $cur = commerce_currency_get_symbol($top_payment['currency']);

  if ($form_state['input']) {
    $selected_method = $form_state['input']['payment_method'];
    foreach ($payment_settings as $payment_setting) {
      if ($payment_setting['payment_method'] == $selected_method) {
        $cur = commerce_currency_get_symbol($payment_setting['currency']);
        continue;
      }
    }
  }

  $donation_amounts = array();
  foreach ($settings['settings']['donation_settings']['donation_amounts'] as $amount => $description) {
    $donation_amounts[$amount * 100] = $description;
  }

  switch ($settings['settings']['donation_settings']['donation_entry']) {
    case 'select':
      $form['donation'] = array(
        '#type' => 'select',
        '#title' => t('Donation (@cur)', array('@cur' => $cur)),
        '#required' => TRUE,
        '#options' => $donation_amounts,
        '#multiple' => FALSE,
      );
      break;

    case 'select_other':
    case 'radio_other':

      $form['donation'] = array(
        '#type' => 'select_or_other',
        '#title' => t('Donation (@cur)', array('@cur' => $cur)),
        '#required' => TRUE,
        '#multiple' => FALSE,
        '#options' => $donation_amounts,
        '#other' => t('Other'),
        '#other_description' => t('Please enter an amount.'),
        '#other_unknown_defaults' => 'other',
        '#other_delimiter' => ', ',
        '#select_type' => $settings['settings']['donation_settings']['donation_entry'] == 'select_other' ? 'select' : 'radios',
      );

      break;

    default:
      $form['donation'] = array(
        '#type' => 'textfield',
        '#title' => t('Donation (@cur)', array('@cur' => $cur)),
        '#default_value' => '',
        '#required' => TRUE,
        '#description' => t('Please enter an amount.'),
      );
      break;
  }

  $products = commerce_product_load_multiple(
    array_merge(
      array(
        $commerce_settings['product_options']['one_time_product'],
      ),
      $commerce_settings['product_options']['recurring']
    )
  );
  if (count($products) > 1) {
    $descriptions = array();
    $product_list = array();
    foreach ($products as $product_id => $product) {
      $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
      $product_list[$product_id] = $product->title;
      $descriptions[$product_id] = redhen_donation_product_description($product_wrapper);
    }
    $product_label = $commerce_settings['payment_options']['product_options']['product_select_label'] == "" ?
      t('Donation Type') :
      $settings['settings']['commerce']['product_options']['product_select_label'];
    $form['product'] = array(
      '#type' => 'radios',
      '#title' => $product_label,
      '#options' => $product_list,
      '#required' => TRUE,
    );
    foreach ($form['product']['#options'] as $key => $label) {
      $form['product'][$key]['#description'] = $descriptions[$key];
    }
  }
  else {
    $product = current($products);
    $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
    $form['product'] = array(
      '#type' => 'hidden',
      '#value' => $product->product_id,
    );
    $form['product_label'] = array(
      '#type' => 'item',
      '#title' => $product->title,
      '#markup' => redhen_donation_product_description($product_wrapper),
    );
  }

  $billing_user = !is_null($contact_object->uid) ? $contact_object->uid : $user->uid;
  $billing = commerce_customer_profile_new('billing', $billing_user);
  // Set default contact address.
  if (!(is_null($contact_object->contact_id)) && isset($settings['settings']['commerce']['profile_selector'])) {
    $billing_address_field = $settings['settings']['commerce']['profile_selector'];
    $billing->commerce_customer_address = $contact_object->{$billing_address_field};
  }

  field_attach_form('commerce_customer_profile', $billing, $form, $form_state);

  $form['commerce_customer_address'][LANGUAGE_NONE][0]['name_block']['#access'] = FALSE;
  $form['commerce_customer_address'][LANGUAGE_NONE][0]['#title'] = t('Billing address');
  $form['commerce_customer_address']['#weight'] = 49;

  if (isset($form_state['values'])) {
    $default_method = $form_state['values']['payment_method'];
  }
  else {
    reset($options);
    $default_method = key($options);
  }

  // Set the default value for the payment method radios.
  $pane_form['payment_method']['#default_value'] = $default_method;

  $payment_method = $payment_methods[$default_method];

  $checkout_pane = array();
  $order = new stdClass();
  $pane_values = array();
  if ($callback = commerce_payment_method_callback($payment_method, 'submit_form')) {
    $pane_form['payment_details'] = $callback($payment_method, $pane_values, $checkout_pane, $order);
  }
  else {
    $pane_form['payment_details'] = array();
  }

  $pane_form['payment_details']['#tree'] = TRUE;
  $pane_form['payment_details']['#prefix'] = '<div id="payment-details">';
  $pane_form['payment_details']['#suffix'] = '</div>';

  $form['commerce_payment_form'] = array(
    '#type' => 'fieldset',
    '#weight' => 50,
    '#title' => t('Payment Information'),
  );

  $form['commerce_payment_form'] = array_merge($form['commerce_payment_form'], $pane_form);

  return $form;
}

/**
 * Validation callback for redhen_donation_form().
 */
function redhen_donation_form_validate($form, &$form_state) {
  $donation = $form_state['donation'];

  // Email validation.
  $valid_email = valid_email_address($form_state['values']['email']);
  if (!$valid_email) {
    $message = t('"%mail" is not a valid email address', array(
      '%mail' => $form_state['values']['email'])
    );
    form_set_error('email', $message);
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('redhen_donation', $donation, $form, $form_state);
}

/**
 * Submit callback for redhen_donation_form().
 */
function redhen_donation_form_submit($form, &$form_state) {
  global $user;
  $settings = $form_state['donation_settings'];
  $values = $form_state['values'];
  $donation = $form_state['donation'];
  $donation_type = redhen_donation_type_load($donation->bundle());

  // Contact and User handling.
  $contact = $form_state['redhen_contact'];
  $new_contact = isset($contact->is_new) && $contact->is_new === TRUE;

  // Does a contact already exist with that email address?
  if ($new_contact) {
    // Attempt to load contact based on given email.
    if ($contact_email_search = redhen_contact_load_by_mail($form_state['values']['email'])) {
      $contact = current($contact_email_search);
    }
  }

  $contact_wrapper = entity_metadata_wrapper('redhen_contact', $contact);

  // If set to update fields, do so. Or if new contact create contact.
  if ($donation_type->settings['contact_settings']['update_fields'] == 1 || $new_contact) {
    // Save default parameters back into the $contact object.
    $contact->first_name = $form_state['values']['first_name'];
    $contact->middle_name = $form_state['values']['middle_name'];
    $contact->last_name = $form_state['values']['last_name'];

    // Only update if new.
    $contact->author_uid = $new_contact ? $contact->author_uid : $user->uid;

    // Notify field widgets.
    field_attach_submit('redhen_contact', $contact, $form, $form_state);

    // Set Address field if needed.
    if ($donation_type->settings['contact_settings']['profile_selector'] != 'billing') {
      $sync_field = $donation_type->settings['contact_settings']['profile_selector'];
      $contact_address = empty($contact_wrapper->{$sync_field}) ? array() : $contact_wrapper->{$sync_field}->value();
      $billing_address = $form_state['values']['commerce_customer_address'][LANGUAGE_NONE][0];
      $updated_address = array_merge($contact_address, $billing_address);
      $contact_wrapper->{$sync_field}->set($updated_address);
    }

    // Set email address.
    $contact_wrapper->email->set($form_state['values']['email']);

    watchdog('redhen_donation', 'RedHen Contact %contact_id has been updated.',
      array('%contact_id' => $contact->contact_id), WATCHDOG_NOTICE
    );
  }

  // Load product.
  $product_id = $values['product'];
  $product = commerce_product_load($product_id);
  $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
  $recurring_product = commerce_recurring_product_is_recurring($product);

  // If recurring and no user is found, create user and link to contact.
  if ($user->uid == 0) {
    $donation_user = user_load_by_mail($form_state['values']['email']);
  }
  else {
    $donation_user = $user;
  }
  if ($recurring_product && $contact_wrapper->user->value() == NULL) {
    if (!$donation_user) {
      $password = user_password();
      $fields = array(
        'name' => $form_state['values']['email'],
        'mail' => $form_state['values']['email'],
        'pass' => $password,
        'status' => 1,
        'init' => $form['values']['email'],
        'roles' => array(
          DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        ),
      );
      $donation_user = user_save('', $fields);
    }
    $contact_wrapper->user->set($donation_user);
    watchdog('redhen_donation', 'User has been linked to the contact %name.',
      array('%name' => $contact->label()), WATCHDOG_NOTICE
    );
  }

  // Save the contact.
  redhen_contact_save($contact);

  // Commerce handling.
  $order_type
    = isset($donation_type->settings['commerce_settings']['order_type']) ?
      $donation_type->settings['commerce_settings']['order_type'] :
      'commerce_order';
  $line_item_type
    = isset($donation_type->settings['commerce_settings']['line_item_type']) ?
      $donation_type->settings['commerce_settings']['line_item_type'] :
      'product';
  $order_uid = $donation_user->uid;
  $order = commerce_cart_order_new($order_uid, $order_type);
  commerce_order_save($order);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $line_item = commerce_product_line_item_new($product, 1, $order->order_id, array(), $line_item_type);
  switch ($settings['settings']['donation_settings']['donation_entry']) {
    case 'select':
      $donation_amount = $form_state['input']['donation'];
      break;

    case 'select_other':
    case 'radio_other':
      if ($form_state['input']['donation']['other'] == '') {
        $donation_amount = $form_state['input']['donation']['select'];
      }
      else {
        $clean_amount = preg_replace("/[^0-9.]/", "", $form_state['input']['donation']['other']);
        $donation_amount = $clean_amount * 100;
      }
      break;

    case 'custom':
      $clean_amount = preg_replace("/[^0-9.]/", "", $form_state['input']['donation']);
      $donation_amount = $clean_amount * 100;
  }

  $payment_methods = $donation_type->settings['commerce_settings']['payment_methods'];
  foreach ($payment_methods as $payment_method) {
    if ($payment_method['payment_method'] == $form_state['values']['payment_method']) {
      $currency = $payment_method['currency'];
      break;
    }
  }

  redhen_donation_update_line_item_price($line_item, $donation_amount, $currency);
  // Save the line item to get the ID.
  commerce_line_item_save($line_item);
  // Add the item to the list of line items.
  $order_wrapper->commerce_line_items[] = $line_item;

  $customer_profile = commerce_customer_profile_new('billing', $donation_user->uid);
  $customer_profile->commerce_customer_address[LANGUAGE_NONE][0] = array();
  $billing_address =& $customer_profile->commerce_customer_address[LANGUAGE_NONE][0];
  $form_state['values']['commerce_customer_address']['und'][0]['name_line'] = $form_state['values']['first_name'] . ' ' . $form_state['values']['last_name'];
  $billing_address = array_merge($billing_address, $form_state['values']['commerce_customer_address'][LANGUAGE_NONE][0]);

  commerce_customer_profile_save($customer_profile);
  $order_wrapper->commerce_customer_billing = $customer_profile;
  $order_wrapper->save();
  commerce_order_calculate_total($order);
  $order_total = $order_wrapper->commerce_order_total->amount->value();

  // Now we can process the payment at last.
  $payment_method = commerce_payment_method_instance_load($form_state['values']['payment_method']);
  // Delegate submit to the payment method callback.
  if ($callback = commerce_payment_method_callback($payment_method, 'submit_form_submit')) {
    $charge = array(
      'amount' => $order_total,
      'currency_code' => $currency,
    );
    // Payment method form.
    $details_form = !empty($form['commerce_payment_form']['payment_details']) ? $form['commerce_payment_form']['payment_details'] : array();
    // Payment method values.
    $details_values = !empty($form_state['values']['payment_details']) ? $form_state['values']['payment_details'] : $form_state['input']['payment_details'];

    // If recurring check if COF.
    if ($recurring_product) {
      $default_cof = commerce_cardonfile_load_multiple_by_uid($donation_user->uid, NULL, TRUE);
      if (!$default_cof) {
        $details_values['cardonfile_instance_default'] = TRUE;
        $details_values['cardonfile'] = 'new';
        $details_values['credit_card']['cardonfile_store'] = TRUE;
        $details_values['credit_card']['default'] = TRUE;
      }
    }

    $callback($payment_method, $details_form, $details_values, $order, $charge);
    $transaction = entity_load('commerce_payment_transaction', FALSE, array('order_id' => $order->order_id));
    $transaction = current($transaction);
    if ($transaction->status == 'success') {
      commerce_order_status_update($order, 'completed', FALSE, TRUE, t('Payment accepted successful.'));
      commerce_line_item_save($line_item);
    }
    else {
      commerce_order_status_update($order, 'canceled', FALSE, TRUE, t('Payment declined.'));
    }
  }

  // Donation entity handling.
  $donation->author_uid = $user->uid;
  $donation->contact_id = $contact->contact_id;

  if ($recurring_product) {
    $pledged = redhen_donation_pledged($order_total, $product_wrapper);
    $received = $order_total;
  }
  else {
    $pledged = $received = $order_total;
  }
  $donation->pledged = $pledged;
  $donation->received = $received;

  $order_id = $order_wrapper->order_id->value();
  if ($recurring_product) {
    $donation->transaction_entity_type = 'commerce_recurring';
    $recurring_entity = current(commerce_recurring_load_by_order($order));
    $donation->transaction_entity_id = $recurring_entity->id;
  }
  else {
    $donation->transaction_entity_type = 'commerce_order';
    $donation->transaction_entity_id = $order_id;
  }

  // Notify field widgets.
  field_attach_submit('redhen_donation', $donation, $form, $form_state);

  // Save the donation and redirect.
  if (redhen_donation_save($donation)) {
    watchdog('redhen_donation', 'RedHen Donation %donation_id has been saved.',
      array('%donation_id' => $donation->donation_id),
      WATCHDOG_NOTICE
    );
    if (!empty($settings['settings']['donation_settings']['confirmation_message'])) {
      $message = $settings['settings']['donation_settings']['confirmation_message'];
      drupal_set_message($message);
    }
    if (!empty($settings['settings']['donation_settings']['confirmation_page'])) {
      $form_state['redirect'] = array($settings['settings']['donation_settings']['confirmation_page']);
    }
    else {
      $host_entity = entity_metadata_wrapper($settings['entity_type'], $settings['entity_id']);
      $form_state['redirect'] = array($host_entity->url->value());
    }
  }
  else {
    drupal_set_message(t('There was a problem submitting your donation.'));
    watchdog('redhen_donation', 'There was a problem with submitting a RedHen Donation for RedHen Contact %contact_id',
      array('%contact_id' => $contact->contact_id),
      WATCHDOG_NOTICE
    );
  }
}

/**
 * Themes the payment methods on a fieldmap edit form into a table.
 */
function theme_redhen_donation_payment_options_form_table($variables) {
  $elements = $variables['elements'];

  // Build the rows array.
  $columns = isset($elements['#columns']) ? $elements['#columns'] : (isset($elements['#header']) ? array_keys($elements['#header']) : array());
  $rows = array();
  foreach (element_children($elements) as $child_key) {
    $child = & $elements[$child_key];
    $data = array();
    $row_columns = empty($columns) ? element_children($child) : $columns;
    foreach ($row_columns as $column) {
      if (isset($child[$column])) {
        $cell = array('data' => drupal_render($child[$column]));
        if (isset($child[$column]['#attributes'])) {
          foreach ($child[$column]['#attributes'] as $key => $value) {
            $cell[$key] = ($key == 'id') ? (is_array($value) ? array($value[0] . '-cell') : $value . '-cell') : $value;
          }
        }
        $data[] = $cell;
      }
    }
    $row = array('data' => $data);
    if (isset($child['#attributes'])) {
      foreach ($child['#attributes'] as $key => $value) {
        $row[$key] = $value;
      }
    }
    $rows[] = $row;
  }

  $config = array(
    'rows' => $rows,
  );
  if (isset($elements['#header'])) {
    $config['header'] = $elements['#header'];
  }
  if (isset($elements['#attributes']) && is_array($elements['#attributes'])) {
    $config['attributes'] = $elements['#attributes'];
  }

  return theme('table', $config);
}

/**
 * Payment method selector helper.
 *
 * @param array $form_state
 *   Form state array.
 *
 * @return array|null
 *   Array of payment methods or null if no methods found.
 */
function redhen_donation_get_payment_methods(&$form_state) {
  $methods = NULL;
  if (isset($form_state['methods'])) {
    $methods = &$form_state['methods'];
  }

  if (empty($methods)) {
    $methods = array(array());
  }
  if (isset($form_state['triggering_element'])) {
    if ($form_state['triggering_element']['#name'] == 'donation_add_field') {
      $methods[] = array();
    }
    if (isset($form_state['triggering_element']['#ajax']['delta']) &&
      $form_state['triggering_element']['#name'] == 'delete_method-' . $form_state['triggering_element']['#ajax']['delta']) {
      $delta = $form_state['triggering_element']['#ajax']['delta'];
      unset($methods[$delta]);
    }
  }

  // Ensure one method exists.
  $empty  = array_filter($methods);
  $methods = empty($methods) && empty($empty) ? array(array()) : $methods;

  return $methods;
}

/**
 * Donation form AJAX callback.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   AJAX command array.
 */
function redhen_donation_form_refresh(&$form, &$form_state) {
  $html1 = drupal_render($form['commerce_payment_form']['payment_details']);
  $html2 = drupal_render($form['donation']);
  $result = array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('#payment-details', $html1),
      ajax_command_replace('.form-item-donation', $html2),
    ),
  );
  return $result;
}

/**
 * Donation line item update callback.
 *
 * @param object $line_item
 *   Commerce Line Item.
 * @param string $decimal_amount
 *   Item amount.
 * @param string $currency
 *   Item currency.
 */
function redhen_donation_update_line_item_price(&$line_item, $decimal_amount, $currency = 'USD') {
  $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $wrapper->commerce_unit_price->currency_code = $currency;

  $wrapper->commerce_unit_price->amount->set($decimal_amount);
  $wrapper->commerce_unit_price->data = commerce_price_component_delete(0, 'base_price');

  $wrapper->commerce_unit_price->data = commerce_price_component_add(
    $wrapper->commerce_unit_price->value(),
    'base_price',
    array(
      'amount' => $decimal_amount,
      'currency_code' => $currency,
      'data' => array(),
    ),
    TRUE
  );
}

/**
 * Production description helper callback.
 *
 * @param object $product_wrapper
 *   Product Entity Metadata wrapper.
 *
 * @return null|string
 *   Product description.
 */
function redhen_donation_product_description($product_wrapper) {
  $description = '';
  if ($product_wrapper->type->value() == 'recurring') {
    $period = $product_wrapper->commerce_recurring_rec_period->period->value();
    $interval = $product_wrapper->commerce_recurring_rec_period->interval->value();
    $end_period = $product_wrapper->commerce_recurring_end_period->value() ?
      $product_wrapper->commerce_recurring_end_period->period->value() : '';
    $end_interval = $product_wrapper->commerce_recurring_end_period->value() ?
      $product_wrapper->commerce_recurring_end_period->interval->value() : '';
    $description = t('@interval recurrence per @period', array(
      '@interval' => $interval,
      '@period' => $period,
    ));
    if ($end_period != '') {
      $description .= ' ' . t('for @end_interval @end_period(s)', array(
        '@end_period' => $end_period,
        '@end_interval' => $end_interval,
      ));
    }

  }
  return $description;
}

/**
 * Helper function to determine amount pledged.
 *
 * @param string $order_total
 *   Order total.
 * @param object $product_wrapper
 *   Product Entity Metatdata wrapper.
 *
 * @return mixed
 *   Pledged amount.
 */
function redhen_donation_pledged($order_total, $product_wrapper) {
  $period = $product_wrapper->commerce_recurring_rec_period->period->value();
  $interval = $product_wrapper->commerce_recurring_rec_period->interval->value();
  $end_period = $product_wrapper->commerce_recurring_end_period->value() ?
    $product_wrapper->commerce_recurring_end_period->period->value() : '';
  $end_interval = $product_wrapper->commerce_recurring_end_period->value() ?
    $product_wrapper->commerce_recurring_end_period->interval->value() : '';

  $duration_array = array(
    'day' => 365,
    'week' => 52,
    'month' => 12,
    'year' => 1,
  );
  $interval_count = $duration_array[$period] / $interval;
  if ($end_period != '') {
    $interval_per_year = 1 / ($duration_array[$end_period] / $end_interval);
    $recurring_count = round($interval_count * $interval_per_year);
    $return = $order_total * $recurring_count;
  }
  else {
    $return = $order_total;
  }

  return $return;
}
