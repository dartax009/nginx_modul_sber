#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_string.h>

#include "ver_json/check_shem_json.h"


static ngx_int_t ngx_http_sber_modul_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_content_sber_modul_init (ngx_conf_t *cf);
static char *ngx_http_conf_sber_modul_init (ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static uint8_t check_on = 0;    ///< 0 - выключена обработка, 1 - включена обработка

typedef struct
{
    char *str;
}ngx_http_some_module_loc_conf_t ;

static ngx_command_t ngx_http_sber_modul_commands[] = {
    { ngx_string("character_shem_check"),
      NGX_CONF_TAKE1|NGX_DIRECT_CONF|NGX_ANY_CONF,
      ngx_http_conf_sber_modul_init,
      0,
      0,
      NULL },
    ngx_null_command
};

static ngx_http_module_t ngx_http_sber_modul_module_ctx = {
    NULL,                          /* preconfiguration */
    ngx_http_content_sber_modul_init,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    NULL,                          /* create location configuration */
    NULL                           /* merge location configuration */
};

ngx_module_t ngx_http_sber_modul_module = {
    NGX_MODULE_V1,
    &ngx_http_sber_modul_module_ctx,  /* module context */
    ngx_http_sber_modul_commands,     /* module directives */
    NGX_HTTP_MODULE,                  /* module type */
    NULL,                             /* init master */
    NULL,                             /* init module */
    NULL,                             /* init process */
    NULL,                             /* init thread */
    NULL,                             /* exit thread */
    NULL,                             /* exit process */
    NULL,                             /* exit master */
    NGX_MODULE_V1_PADDING
};

static void ngx_http_sber_body_hendler(ngx_http_request_t *r)
{
    ngx_buf_t *buf;
    ngx_chain_t out;
    ngx_int_t rc;
    ngx_str_t response;
    ngx_int_t status = NGX_HTTP_OK;

    if ( check_shem((char *)r->request_body->bufs->buf->pos, r->request_body->bufs->buf->last - r->request_body->bufs->buf->pos) == 1 )
    {
        ngx_http_finalize_request(r, NGX_HTTP_UNSUPPORTED_MEDIA_TYPE);
        return;
    }
    else
    {
        response.data = (u_char *)"The character is saved";
        response.len = sizeof("The character is saved")-1;
    }

    r->headers_out.status = status;
    r->headers_out.content_length_n = response.len;

    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
    {
        ngx_http_finalize_request(r, rc);
        return;
    }

    buf = ngx_create_temp_buf(r->pool, response.len);
    if (buf == NULL)
    {
        ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
        return;
    }

    ngx_memcpy(buf->pos, response.data, response.len);
    buf->last = buf->pos + response.len;
    buf->last_buf = 1;

    out.buf = buf;
    out.next = NULL;

    ngx_http_output_filter(r, &out);
}

static ngx_int_t ngx_http_sber_modul_handler(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_str_t json_content_type = ngx_string("application/json");
    u_char *content_type_start;

    if (check_on == 0)
    {
        return NGX_DECLINED; // Чтобы продолжить дальнейший конвеер обработки возвращаем DECLINED
    }

    if (r->method != NGX_HTTP_POST) {
        return NGX_DECLINED;
    }

    if (r->headers_in.content_type == NULL)
    {
        return NGX_DECLINED;
    }
    else
    {
        content_type_start = r->headers_in.content_type->value.data;

        if (ngx_strncmp(content_type_start, (char *)json_content_type.data, json_content_type.len - 1) != 0)
        {
            return NGX_DECLINED;
        }
    }

    rc = ngx_http_read_client_request_body(r, ngx_http_sber_body_hendler);
    if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
    }

    return NGX_DONE;
}

static ngx_int_t ngx_http_content_sber_modul_init (ngx_conf_t *cf)
{
    ngx_http_handler_pt *h = NULL;
    ngx_http_core_main_conf_t *cmcf = NULL;


    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    if (h == NULL)
    {
        return NGX_ERROR;
    }

    (*h) = ngx_http_sber_modul_handler;

    return NGX_OK;
}

static char *ngx_http_conf_sber_modul_init (ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_str_t *value = cf->args->elts;

    if (ngx_strncmp((char *)value[1].data, "on", sizeof("on")-1) == 0)
    {
        check_on = 1;
        return NGX_CONF_OK;
    }

    if (ngx_strncmp((char *)value[1].data, "off", sizeof("off")-1) == 0)
    {
        return NGX_CONF_OK;
    }

    return NGX_CONF_ERROR;
}